#.rst:
# Find SystemC
# ------------
# This module finds if SystemC is installed and determines where the
# include files and libraries are.
#
# This will define the following variables::
#
#   SystemC_FOUND           - True if the system has the SystemC library
#   SystemC_VERSION         - The version of the SystemC library which was found
#   SystemC_INCLUDE_DIRS    - Full paths to all include dirs.
#   SystemC_LIBRARIES       - Full paths to all libraries.
#
# The minimum required version of SystemC can be specified using the
# standard CMake syntax, e.g. find_package(SystemC 2.2)
#
# Example Usages::
#   find_package(SystemC)
#   find_package(SystemC 2.3)
#

#=============================================================================
# Copyright 2009-2016 Jens Rudolf
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

# First, we try to use pkg-config to find the library. We cannot rely on this,
# as it may not be available, but it provides a good starting point.
find_package(PkgConfig)
pkg_check_modules(PC_SystemC QUIET SystemC)


# The HINTS option should only be used for values computed from the system.
SET(_SystemC_HINTS
    ${SYSTEMC_HOME}/include
    ${SYSTEMC_HOME}/lib
    ${SYSTEMC_HOME}/lib-linux
    ${SYSTEMC_HOME}/lib-linux64
    ${SYSTEMC_HOME}/lib-macos
    $ENV{SYSTEMC_HOME}/include
    $ENV{SYSTEMC_HOME}/lib
    $ENV{SYSTEMC_HOME}/lib-linux
    $ENV{SYSTEMC_HOME}/lib-linux64
    $ENV{SYSTEMC_HOME}/lib-macos
    ${CMAKE_INSTALL_PREFIX}/include
    ${CMAKE_INSTALL_PREFIX}/lib
    ${CMAKE_INSTALL_PREFIX}/lib-linux
    ${CMAKE_INSTALL_PREFIX}/lib-linux64
    ${CMAKE_INSTALL_PREFIX}/lib-macos
    )

# Hard-coded guesses should still go in PATHS. This ensures that the user
# environment can always override hard guesses.
SET(_SytemC_PATHS
    /usr/include/systemc
    /usr/lib
    /usr/lib-linux
    /usr/lib-linux64
    /usr/lib-macos
    /usr/local/lib
    /usr/local/lib-linux
    /usr/local/lib-linux64
    /usr/local/lib-macos
    ${PC_SystemC_INCLUDE_DIRS}
    ${PC_SystemC_LIBRARY_DIRS}
    )

find_path(SystemC_INCLUDE_DIR
    NAMES systemc.h
    PATHS ${_SystemC_PATHS}
    HINTS ${_SystemC_HINTS}
    PATH_SUFFIXES systemc
)

find_library(SystemC_LIBRARY
    NAMES systemc
    PATHS ${_SystemC_PATHS}
    HINTS ${_SystemC_HINTS}
)

if(PC_SystemC_VERSION)
    # Get the version info from pkg-config variables
    set(SystemC_VERSION ${PC_SystemC_VERSION})
    set(SystemC_VERSION_MAJOR ${PC_SystemC_VERSION_MAJOR})
    set(SystemC_VERSION_MINOR ${PC_SystemC_VERSION_MINOR})
    set(SystemC_VERSION_PATCH ${PC_SystemC_VERSION_PATCH})
else()
    # No package config, we need to find the version file
    find_file(SystemC_VERSION_FILE
        NAMES sc_ver.h
        PATHS ${_SystemC_PATHS}
        HINTS ${_SystemC_HINTS}
        PATH_SUFFIXES sysc/kernel
    )
    file(READ ${SystemC_VERSION_FILE} _VERSION_FILE_CONTENTS)
    string(REGEX MATCH "#define SC_VERSION_MAJOR[ \t]*[0-9]+"
        SystemC_VERSION_MAJOR "${_VERSION_FILE_CONTENTS}")
    string(REGEX REPLACE "#define SC_VERSION_MAJOR[ \t ]*([0-9]+)" "\\1"
        SystemC_VERSION_MAJOR "${SystemC_VERSION_MAJOR}") 
    string(REGEX MATCH "#define SC_VERSION_MINOR[ \t]*[0-9]+"
        SystemC_VERSION_MINOR "${_VERSION_FILE_CONTENTS}")
    string(REGEX REPLACE "#define SC_VERSION_MINOR[ \t]*([0-9]+)" "\\1"
        SystemC_VERSION_MINOR "${SystemC_VERSION_MINOR}") 
    string(REGEX MATCH "#define SC_VERSION_PATCH[ \t]*[0-9]+"
        SystemC_VERSION_PATCH "${_VERSION_FILE_CONTENTS}")
    string(REGEX REPLACE "#define SC_VERSION_PATCH[ \t]*([0-9]+)" "\\1"
        SystemC_VERSION_PATCH "${SystemC_VERSION_PATCH}") 

    set(SystemC_VERSION
        "${SystemC_VERSION_MAJOR}.${SystemC_VERSION_MINOR}.${SystemC_VERSION_PATCH}"
    )
endif()

message(STATUS "SystemC library: ${SystemC_LIBRARY}")
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SystemC
    REQUIRED_VARS
        SystemC_LIBRARY
        SystemC_INCLUDE_DIR
    VERSION_VAR SystemC_VERSION
)

if(SYSTEMC_FOUND)
    set(SystemC_LIBRARIES ${SystemC_LIBRARY})
    set(SystemC_INCLUDE_DIRS ${SystemC_INCLUDE_DIR})
    set(SystemC_DEFINITIONS ${PC_SystemC_CFLAGS_OTHER})
endif()

# Hide internal values from cache
mark_as_advanced(SystemC_INCLUDE_DIR SystemC_LIBRARY)
