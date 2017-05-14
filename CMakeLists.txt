# ============================================================================
#
# Copyright 2009-2016 University of Rostock
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#
# ============================================================================
cmake_minimum_required(VERSION 2.8)


project("Zynq MP3" CXX)

# Set the module path in order to find the custom modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


find_package(ALSA)
find_package(SystemC REQUIRED 2.3)
find_package(Vivado REQUIRED 2014.4 COMPONENT HLS DS)


# We need to include the SystemC headers
include_directories(${SystemC_INCLUDE_DIRS})


# The names and location of the unix pipes to be created
set(HW_INPUT_FIFO "${CMAKE_BINARY_DIR}/HW_INPUT_FIFO")
set(HW_OUTPUT_FIFO "${CMAKE_BINARY_DIR}/HW_OUTPUT_FIFO")


# This is required for GCC >= 6.1
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ------------------------ SystemC Model -------------------------------------
#
# This is the simulation (model) executable. Change this according to your
# source file names and paths.
#
add_executable(model
    "${CMAKE_SOURCE_DIR}/src/Filter.cpp"
    "${CMAKE_SOURCE_DIR}/src/Consumer.cpp"
    "${CMAKE_SOURCE_DIR}/src/Producer.cpp"
    "${CMAKE_SOURCE_DIR}/src/Testbench.cpp"
)

# Link the testbench against the SystemC library.
target_link_libraries(model ${SystemC_LIBRARIES})

# ----------------------------------------------------------------------------



# --------------------------- MP3 Player -------------------------------------
#
# This is the player executable. Change this according to your source file
# names and paths.
#
add_executable(player
    "${CMAKE_SOURCE_DIR}/src/minimp3.cpp"
    "${CMAKE_SOURCE_DIR}/src/player_alsa.cpp"
)

target_link_libraries(player ${ALSA_LIBRARY})
# ----------------------------------------------------------------------------

# Play MP3 file
add_custom_target(
    play
    COMMAND $<TARGET_FILE:player> "${CMAKE_SOURCE_DIR}/Suerte.mp3"
    DEPENDS player
)

# Start SystemC simulation
add_custom_target(
    sim
    COMMAND $<TARGET_FILE:model>
    DEPENDS model
)



# Create FIFOs
add_custom_command(
    OUTPUT ${HW_INPUT_FIFO}
    COMMAND "mkfifo" ${HW_INPUT_FIFO}
    COMMENT "Creating pipe ${HW_INPUT_FIFO}"
)

add_custom_command(
    OUTPUT ${HW_OUTPUT_FIFO}
    COMMAND "mkfifo" ${HW_OUTPUT_FIFO}
    COMMENT "Creating pipe ${HW_OUTPUT_FIFO}"
)


# ------------------------- Vivado HLS --------------------------------------------------

set(HLS_DIR "${CMAKE_BINARY_DIR}/ZynqMP3")
set(TCL_DIR "${CMAKE_SOURCE_DIR}/tcl")


# Setup HLS project
add_custom_command(
    OUTPUT "${HLS_DIR}/vivado_hls.app"
    COMMAND ${Vivado_HLS_EXECUTABLE} "-f" "${TCL_DIR}/hls_setup.tcl"
    COMMENT "Creating Vivado HLS project"
)

# Start Vivado C Simulation
add_custom_target(
    csim
    COMMAND ${Vivado_HLS_EXECUTABLE} "-f" "${TCL_DIR}/hls_csim.tcl"
    DEPENDS "${HLS_DIR}/vivado_hls.app"
    COMMENT "Starting Vivado C Simulation"

)

# Open Vivado_HLS GUI
add_custom_target(
    hlsgui
    COMMAND ${Vivado_HLS_EXECUTABLE} "-p" ${HLS_DIR}
    COMMENT "Opening Vivado HLS GUI"
    DEPENDS "${HLS_DIR}/vivado_hls.app"
)


# Start the High-Level Synthesis
add_custom_target(
    hls
    COMMAND ${Vivado_HLS_EXECUTABLE} "-f" "${TCL_DIR}/hls_csynth.tcl"
    DEPENDS "${HLS_DIR}/vivado_hls.app"
    COMMENT "Starting High-Level Synthesis"
)


# Start the RTL Co-Simulation
add_custom_target(
    cosim
    COMMAND ${Vivado_HLS_EXECUTABLE} "-f" "${TCL_DIR}/hls_cosim.tcl"
    COMMENT "Starting RTL Co-Simulation"
    DEPENDS hls
)


# Export the Vivado IP package
add_custom_target(
    export
    COMMAND ${Vivado_HLS_EXECUTABLE} "-f" "${TCL_DIR}/hls_export.tcl"
    COMMENT "Exporting IP package"
    DEPENDS cosim
)


# Setup Vivado RTL project
add_custom_command(
    OUTPUT "${CMAKE_SOURCE_DIR}/verilog/vivado/xillydemo.xpr"
    COMMAND
        ${Vivado_EXECUTABLE} "-mode" "batch"
        "-source" "${CMAKE_SOURCE_DIR}/verilog/xillydemo-vivado.tcl"
    COMMENT "Creating Vivado DS project"
)


# Run Vivado RTL Synthesis
add_custom_target(
    vivado
    COMMAND
        ${Vivado_EXECUTABLE} "${CMAKE_SOURCE_DIR}/verilog/vivado/xillydemo.xpr"
    COMMENT "Starting Vivado DS"
    DEPENDS "${CMAKE_SOURCE_DIR}/verilog/vivado/xillydemo.xpr"
)
