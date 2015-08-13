
# Define the project for SDAccel
create_project -name prj_ocl_pooling  -dir . -force
set_property platform vc690-admpcie7v3-1ddr-gen2 [current_project]

# Host Compiler Flags
set_property -name host_cflags -value "-g -Wall -D FPGA_DEVICE -D C_KERNEL" -objects [current_project]

# Host Source Files
add_files "main.c"
add_files "pool3_max_layer.h"
set_property file_type "c header files" [get_files "pool3_max_layer.h"]

# Kernel Definition
create_kernel pool3_max_layer -type c
add_files -kernel [get_kernels pool3_max_layer] "pool3_max_layer.c"

# Define Binary Containers
create_opencl_binary -device [lindex [get_device "fpga0"] 0] pool3_max_layer
set_property region "OCL_REGION_0" [get_opencl_binary pool3_max_layer]
create_compute_unit -opencl_binary [get_opencl_binary pool3_max_layer] -kernel [get_kernels pool3_max_layer] -name ocl_pooling

# Compile the design for CPU based emulation
compile_emulation -flow cpu -opencl_binary [get_opencl_binary pool3_max_layer]

# Run the compiled application in CPU based emulation mode
# run_emulation -flow cpu -args "pool3_max_layer.xclbin"

report_estimate

# Compile the application to run on the accelerator card
#build_system
#
# Package the application binaries
#package_system

