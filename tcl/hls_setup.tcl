###############################################################################
#
# setup.tcl - Setup a Vivado HLS project.
#
# Author:   Jens Rudolf (Uni Rostock)
# Date:     2016-07-04
#
# Changelog:
#       2016-07-04 - Initial version
#
# Copyright (c) 2015 by Jens Rudolf (Uni Rostock). All rights
# reserved. For educational use only. NO WARRANTY!
# 
# Report bugs to:
#       jens.rudolf@uni-rostock.de
# 
###############################################################################

set PROJECT_NAME ZynqMP3
set PROJECT_ROOT [pwd]/..

set SOURCES_PATH ${PROJECT_ROOT}/src
set INCLUDE_PATH ${PROJECT_ROOT}/include
set INCLUDES "\
    -I${SOURCES_PATH} \
    -I${INCLUDE_PATH} \
    -I${INCLUDE_PATH}/glibc-2.18 \
    -I${INCLUDE_PATH}/gcc-4.6.3 \
    -I${INCLUDE_PATH}/gcc-4.6.3/c++ \
    -I${INCLUDE_PATH}/gcc-4.6.3/c++/x86_64-unknown-linux-gnu"

set CFLAGS "-nostdinc ${INCLUDES}"

# Open vivado project
open_project -reset ${PROJECT_NAME}

# Set the top level module for synthesis
set_top Filter

# Add the design source files
add_files ${SOURCES_PATH}/Filter.cpp -cflags ${CFLAGS}

# Add the simulation source files
add_files -tb ${SOURCES_PATH}/Consumer.cpp -cflags ${CFLAGS}
add_files -tb ${SOURCES_PATH}/Producer.cpp -cflags ${CFLAGS}
add_files -tb ${SOURCES_PATH}/Testbench.cpp -cflags ${CFLAGS}

# Open the default solution
open_solution -reset "solution1"

# Set target hardware and clock
set_part {xc7z020clg484-1}
create_clock -period 10 -name default

# Close project and exit
close_project
exit
