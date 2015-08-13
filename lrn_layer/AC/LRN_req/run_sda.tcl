
# Define the project for SDAccel
create_project -name prj_ocl_lrn  -dir . -force
set_property platform vc690-admpcie7v3-1ddr-gen2 [current_project]

# Host Compiler Flags
set_property -name host_cflags -value "-lm -g -Wall -D FPGA_DEVICE -D C_KERNEL" -objects [current_project]

# Host Source Files
add_files "main.c"
add_files "lrn1_ac_float.h"
set_property file_type "c header files" [get_files "lrn1_ac_float.h"]

# Kernel Definition
create_kernel lrn1_ac_layer -type c
add_files -kernel [get_kernels lrn1_ac_layer] "lrn1_ac_float.c"

# Define Binary Containers
create_opencl_binary -device [lindex [get_device "fpga0"] 0] lrn1_ac_layer
set_property region "OCL_REGION_0" [get_opencl_binary lrn1_ac_layer]
create_compute_unit -opencl_binary [get_opencl_binary lrn1_ac_layer] -kernel [get_kernels lrn1_ac_layer] -name ocl_lrn

# Compile the design for CPU based emulation
compile_emulation -flow cpu -opencl_binary [get_opencl_binary lrn1_ac_layer]

# Run the compiled application in CPU based emulation mode
run_emulation -flow cpu -args "lrn1_ac_layer.xclbin"

report_estimate

# Compile the application to run on the accelerator card
build_system

# Package the application binaries
package_system

