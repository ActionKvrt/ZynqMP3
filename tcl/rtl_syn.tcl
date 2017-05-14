set project [pwd]/../verilog/vivado/xillydemo.xpr

open_project ${project} 

launch_runs -jobs 4 impl_1 -to_step write_bitstream
wait_on_run impl_1

close_project
exit
