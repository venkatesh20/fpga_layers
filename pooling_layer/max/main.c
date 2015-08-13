#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <CL/opencl.h>
#include "pool3_max_layer.h"


////////////////////////////////////////////////////////////////////////////////

int
load_file_to_memory(const char *filename, char **result)
{ 
  int size = 0;
  FILE *f = fopen(filename, "rb");
  if (f == NULL) 
  { 
    *result = NULL;
    return -1; // -1 means file opening fail 
  } 
  fseek(f, 0, SEEK_END);
  size = ftell(f);
  fseek(f, 0, SEEK_SET);
  *result = (char *)malloc(size+1);
  if (size != fread(*result, sizeof(char), size, f)) 
  { 
    free(*result);
    return -2; // -2 means file reading fail 
  } 
  fclose(f);
  (*result)[size] = 0;
  return size;
}

int main(int argc, char** argv)
{
    int err;                            // error code returned from api calls
     
    float img[IWIDTH*IHEIGHT];
    float out[OWIDTH*OHEIGHT];
    float sw_out[OWIDTH][OHEIGHT], hw_out[OWIDTH][OHEIGHT];

    int pfun=0;
    int err_cnt = 0, count = 0;

  unsigned int correct;               // number of correct results returned

  size_t global[2];                   // global domain size for our calculation
  size_t local[2];                    // local domain size for our calculation

  cl_platform_id platform_id;         // platform id
  cl_device_id device_id;             // compute device id 
  cl_context context;                 // compute context
  cl_command_queue commands;          // compute command queue
  cl_program program;                 // compute program
  cl_kernel kernel;                   // compute kernel
   
  char cl_platform_vendor[1001];
  char cl_platform_name[1001];
   
  cl_mem input;                     // device memory used for the input array
  cl_mem output;                    // device memory used for the output array
   
  if (argc != 2){
    printf("%s <inputfile>\n", argv[0]);
    return EXIT_FAILURE;
  }

   // Fill input with data pattern

    /* Open data file and read it */
    FILE  *fp;
    int x,y;

   fp=fopen("/home/dicecco1/caffe_sdaccel_layers/Pool_Layer/Pooling_modified/max/test.dat","r");

   float tmp;
   for (x=0;x<IWIDTH*IHEIGHT;x++) {
         fscanf(fp, "%f", &tmp);
         img[x]=tmp;
         printf("Input at x is %f\n", tmp);
   }
   fclose(fp);

/*   for (x=0; x<OWIDTH*OHEIGHT; x++) {
     out[x]=0.0;
   }
*/
   // Connect to first platform
  //
  err = clGetPlatformIDs(1,&platform_id,NULL);
  if (err != CL_SUCCESS)
  {
    printf("Error: Failed to find an OpenCL platform!\n");
    printf("Test failed\n");
    return EXIT_FAILURE;
  }
  err = clGetPlatformInfo(platform_id,CL_PLATFORM_VENDOR,1000,(void *)cl_platform_vendor,NULL);
  if (err != CL_SUCCESS)
  {
    printf("Error: clGetPlatformInfo(CL_PLATFORM_VENDOR) failed!\n");
    printf("Test failed\n");
    return EXIT_FAILURE;
  }
  printf("CL_PLATFORM_VENDOR %s\n",cl_platform_vendor);
  err = clGetPlatformInfo(platform_id,CL_PLATFORM_NAME,1000,(void *)cl_platform_name,NULL);
  if (err != CL_SUCCESS)
  {
    printf("Error: clGetPlatformInfo(CL_PLATFORM_NAME) failed!\n");
    printf("Test failed\n");
    return EXIT_FAILURE;
  }
  printf("CL_PLATFORM_NAME %s\n",cl_platform_name);
 
  // Connect to a compute device
  //
  int fpga = 0;
#if defined (FPGA_DEVICE)
  fpga = 1;
#endif
  err = clGetDeviceIDs(platform_id, fpga ? CL_DEVICE_TYPE_ACCELERATOR : CL_DEVICE_TYPE_CPU,
                       1, &device_id, NULL);
  if (err != CL_SUCCESS)
  {
    printf("Error: Failed to create a device group!\n");
    printf("Test failed\n");
    return EXIT_FAILURE;
  }
  
  // Create a compute context 
  //
  context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  if (!context)
  {
    printf("Error: Failed to create a compute context!\n");
    printf("Test failed\n");
    return EXIT_FAILURE;
  }

  // Create a command commands
  //
  commands = clCreateCommandQueue(context, device_id, 0, &err);
  if (!commands)
  {
    printf("Error: Failed to create a command commands!\n");
    printf("Error: code %i\n",err);
    printf("Test failed\n");
    return EXIT_FAILURE;
  }

  int status;

  // Create Program Objects
  //
  
  // Load binary from disk
  unsigned char *kernelbinary;
  char *xclbin=argv[1];
  printf("loading %s\n", xclbin);
  int n_i = load_file_to_memory(xclbin, (char **) &kernelbinary);
  if (n_i < 0) {
    printf("failed to load kernel from xclbin: %s\n", xclbin);
    printf("Test failed\n");
    return EXIT_FAILURE;
  }
  size_t n = n_i;
  // Create the compute program from offline
  program = clCreateProgramWithBinary(context, 1, &device_id, &n,
                                      (const unsigned char **) &kernelbinary, &status, &err);
  if ((!program) || (err!=CL_SUCCESS)) {
    printf("Error: Failed to create compute program from binary %d!\n", err);
    printf("Test failed\n");
    return EXIT_FAILURE;
  }

  // Build the program executable
  //
  err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (err != CL_SUCCESS)
  {
    size_t len;
    char buffer[2048];

    printf("Error: Failed to build program executable!\n");
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
    printf("%s\n", buffer);
    printf("Test failed\n");
    return EXIT_FAILURE;
  }

  // Create the compute kernel in the program we wish to run
  //
  kernel = clCreateKernel(program, "pool3_max_layer", &err);
  if (!kernel || err != CL_SUCCESS)
  {
    printf("Error: Failed to create compute kernel!\n");
    printf("Test failed\n");
    return EXIT_FAILURE;
  }

  // Create the input and output arrays in device memory for our calculation
  //
  input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(float)*IWIDTH*IHEIGHT, NULL, NULL);
  output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float)*OWIDTH*OHEIGHT, NULL, NULL);
  if (!input || !output)
  {
    printf("Error: Failed to allocate device memory!\n");
    printf("Test failed\n");
    return EXIT_FAILURE;
  }    
    
  // Write our data set into the input array in device memory 
  //
  err = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, sizeof(float)*IWIDTH*IHEIGHT, img, 0, NULL, NULL);
  if (err != CL_SUCCESS)
  {
    printf("Error: Failed to write to source array img!\n");
    printf("Test failed\n");
    return EXIT_FAILURE;
  }

   
  // Set the arguments to our compute kernel
  //
  err = 0;
  err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
  err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
  if (err != CL_SUCCESS)
  {
    printf("Error: Failed to set kernel arguments! %d\n", err);
    printf("Test failed\n");
    return EXIT_FAILURE;
  }

  // Execute the kernel over the entire range of our 1d input data set
  // using the maximum number of work group items for this device
  //

#ifdef C_KERNEL
  err = clEnqueueTask(commands, kernel, 0, NULL, NULL);
#endif

  if (err)
  {
    printf("Error: Failed to execute kernel! %d\n", err);
    printf("Test failed\n");
    return EXIT_FAILURE;
  }

  // Read back the results from the device to verify the output
  //
  cl_event readevent;
  err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(float)*OWIDTH*OHEIGHT, out, 0, NULL, &readevent );  
  if (err != CL_SUCCESS)
  {
    printf("Error: Failed to read output array! %d\n", err);
    printf("Test failed\n");
    return EXIT_FAILURE;
  }

  clWaitForEvents(1, &readevent);
    
      
  // Validate our results
  
   // convert from 1d to 2d first
    for (x=0; x<OWIDTH; x++) {
      for (y=0; y<OHEIGHT; y++) {
         hw_out[x][y]=out[count];
         count++;
      }
   }

    // Read reference values
   fp=fopen("/home/dicecco1/caffe_sdaccel_layers/Pool_Layer/Pooling_modified/max/sw_vals.dat", "r");
   for (x=0;x<OWIDTH;x++) {
     for (y=0; y<OHEIGHT; y++) {
       fscanf(fp, "%f", &tmp);
       sw_out[x][y]=tmp;
     }
   } 

   fclose(fp);    
   count=0;

   // compare
    
   for (x=0; x<OWIDTH; x++) {
      for (y=0; y<OHEIGHT; y++) {
         if ((sw_out[x][y]-hw_out[x][y]) > 25) {
              printf("SW VALUE %f, HW VALUE %f\n", sw_out[x][y], hw_out[x][y]);
              err_cnt++;
          }
      }
   }
    
    
  // Shutdown and cleanup
  //
  clReleaseMemObject(input);
  clReleaseMemObject(output);
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(commands);
  clReleaseContext(context);

  if(err_cnt == 0){
    printf("Test passed!\n");
    return EXIT_SUCCESS;
  }
  else{
    printf("Test failed\n");
    return EXIT_FAILURE;
  }
}
