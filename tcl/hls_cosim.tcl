###############################################################################
#
# cosim.tcl - Run Vivado RTL-Cosimulation for a SystemC project.
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


# Open vivado project and the default solution
open_project ${PROJECT_NAME}
open_solution "solution1"

# Run RTL Cosimulation
cosim_design -O -trace_level none -rtl systemc

# Close project and exit Vivado
close_project
exit
