# Define the project for SDAccel
create_project -name example_alpha -dir . -force
set_property platform vc690-admpcie7v3-1ddr-gen2 [current_project]

# Host Compiler Flags
set_property -name host_cflags -value "-g -Wall -D FPGA_DEVICE -D C_KERNEL" -objects [current_project]

# Host Source Files
add_files "test-cl.c"

# Kernel Definition
create_kernel conv3_layer -type c
add_files -kernel [get_kernels conv3_layer] "conv3_layer.c"

# Define Binary Containers
create_opencl_binary -device [lindex [get_device "fpga0"] 0] conv3_layer
set_property region "OCL_REGION_0" [get_opencl_binary conv3_layer]
create_compute_unit -opencl_binary [get_opencl_binary conv3_layer] -kernel [get_kernels conv3_layer] -name k1

# Compile the design for CPU based emulation
compile_emulation -flow cpu -opencl_binary [get_opencl_binary conv3_layer]

# Run the compiled application in CPU based emulation mode
#run_emulation -flow cpu -args "conv3_layer.xclbin"

report_estimate

puts stderr "Building and packaging system"
# Compile the application to run on the accelerator card
build_system

# Package the application binaries
package_system

