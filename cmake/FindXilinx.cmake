# - Try to find Xilinx installation
# Once done this will define
#  Xilinx_FOUND - System has Xilinx installed
#  Xilinx_DIR - Directory where Xilinx tools have been installed

include(FindPackageHandleStandardArgs)

# Find the Xilinx root directory by searching for the xic subdirectory
find_path(
    Xilinx_DIR
    NAMES xic
    PATHS "/opt/Xilinx" "/usr/Xilinx" "/usr/local/Xilinx" "/usr/share/Xilinx"
    ENV XILINX_HOME
    )

# handle the QUIETLY and REQUIRED arguments and set XILINX_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
    Xilinx
    "No Xilinx toolchain installation found"
    Xilinx_DIR
    )

mark_as_advanced(Xilinx_DIR)
