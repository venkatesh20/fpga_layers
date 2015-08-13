
# Define the project for SDAccel
create_project -name prj_ocl_lrn  -dir . -force
set_property platform vc690-admpcie7v3-1ddr-gen2 [current_project]

# Host Compiler Flags
set_property -name host_cflags -value "-g -Wall -D FPGA_DEVICE -D C_KERNEL" -objects [current_project]

# Host Source Files
add_files "main.c"
add_files "lrn1_wc_float.h"
set_property file_type "c header files" [get_files "lrn1_wc_float.h"]

# Kernel Definition
create_kernel lrn1_wc_float -type c
add_files -kernel [get_kernels lrn1_wc_float] "lrn1_wc_float.c"

# Define Binary Containers
create_opencl_binary -device [lindex [get_device "fpga0"] 0] lrn1_wc_float
set_property region "OCL_REGION_0" [get_opencl_binary lrn1_wc_float]
create_compute_unit -opencl_binary [get_opencl_binary lrn1_wc_float] -kernel [get_kernels lrn1_wc_float] -name ocl_lrn

# Compile the design for CPU based emulation
compile_emulation -flow cpu -opencl_binary [get_opencl_binary lrn1_wc_float]

# Run the compiled application in CPU based emulation mode
run_emulation -flow cpu -args "lrn1_wc_float.xclbin"


report_estimate

# Compile the application to run on the accelerator card
#build_system

# Package the application binaries
#package_system

