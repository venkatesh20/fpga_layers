
#Create project
create_project -force -name prj_relu_1
set_property platform vc690-admpcie7v3-1ddr-gen2 [current_project]

#Add host code
add_files relu_1_host.c 
set_property -name host_cflags -value "-g -Wall -D FPGA_DEVICE -D C_KERNEL" -objects [current_project]

#Add host header add_file

#Create binary, kernel, compute unit
create_opencl_binary -device [lindex [get_device fpga0] 0] bin_relu_layer
set_property region OCL_REGION_0 [get_opencl_binary bin_relu_layer]
create_kernel -type c relu_layer
add_files -kernel [get_kernels relu_layer] relu_1_kernel.c
create_compute_unit -opencl_binary [get_opencl_binary bin_relu_layer] -kernel [get_kernels relu_layer] -name relu_layer0

# Run emulation of hardware
compile_emulation -opencl_binary [get_opencl_binary bin_relu_layer] 
#run_emulation -args "bin_relu_layer.xclbin"

report_estimate

#Build and package system
build_system
package_system

