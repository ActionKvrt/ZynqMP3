# - Try to find the Vivado Design Suite
# Once done this will define
#  Vivado_FOUND           - Vivado DS installed on system
#  Vivado_DIR             - The installation directory of Vivado DS
#  Vivado_VERSION         - The tool version of Vivado DS
#  Vivado_EXECUTABLE      - Executable for Vivado Design Suite
#  Vivado_HLS_EXECUTABLE  - Executable for Vivado High-Level Synthesis
#
# ---------------------------------------------------------------------------

# Vivado is part of the Xilinx tools, so find them first
find_package(Xilinx REQUIRED)

# Find the Vivado install directory
find_file(_Vivado_PREFIX NAMES Vivado PATHS ${Xilinx_DIR})

# Find all versions installed and sort them ascending
file(GLOB _Vivado_VERSIONS_AVAIL RELATIVE ${_Vivado_PREFIX} "${_Vivado_PREFIX}/*")
list(SORT _Vivado_VERSIONS_AVAIL)

# By default we use the latest version available
list(GET _Vivado_VERSIONS_AVAIL -1 Vivado_VERSION)

# Check if specific version requested by find_package
if(Vivado_FIND_VERSION)
    set(Vivado_VERSION ${Vivado_FIND_VERSION})
    if(NOT ${Vivado_VERSION} IN_LIST VIVADO_VERSIONS_AVAIL) 
        set(VIVADO_VERSION NOTFOUND)
    endif()
endif()

# handle the QUIETLY and REQUIRED arguments and set VIVADO_FOUND to TRUE
# if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    Vivado
    VERSION_VAR Vivado_VERSION
    REQUIRED_VARS _Vivado_PREFIX Vivado_VERSION
    )

set(Vivado_DIR ${_Vivado_PREFIX}/${Vivado_VERSION})
set(Vivado_EXECUTABLE ${Vivado_DIR}/bin/vivado)
set(Vivado_HLS_EXECUTABLE ${_Vivado_PREFIX}/../Vivado_HLS/${Vivado_VERSION}/bin/vivado_hls)

mark_as_advanced(Vivado_EXECUTABLE Vivado_DIR)
