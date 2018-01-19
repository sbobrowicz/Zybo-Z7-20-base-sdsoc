open_bd_design [get_files */home/digilent/work/git/Zybo-Z7-20-base-sdsoc/src/others/sdsoc/.Xil/vivado/zybo_z7_20.srcs/sources_1/bd/zybo_z7_20/zybo_z7_20.bd]
set fp [open "locked.dat" "w"]
set ips [get_ips]
foreach ip $ips {
    set status [get_property is_locked $ip]
    if {$status} {
        puts $fp "$ip"
    }
}
close $fp

source -notrace /opt/Xilinx/SDx/2017.2/scripts/vivado/sdsoc_pfm.tcl
source -notrace /home/digilent/work/git/Zybo-Z7-20-base-sdsoc/src/others/sdsoc/hw_pfm_gen.tcl
set tmp_vendor [lindex [array get sdsoc::name vendor] 1]
set tmp_name [lindex [array get sdsoc::name name] 1]
set tmp_version [lindex [array get sdsoc::name version] 1]
if {[string equal "" [get_property dsa.vendor   [current_project]]]} { set_property dsa.vendor   "${tmp_vendor}"  [current_project] }
if {[string equal "" [get_property dsa.board_id [current_project]]]} { set_property dsa.board_id "${tmp_name}"    [current_project] }
if {[string equal "" [get_property dsa.name     [current_project]]]} { set_property dsa.name     "${tmp_name}"    [current_project] }
if {[string equal "0.0" [get_property dsa.version  [current_project]]]} { set_property dsa.version  "${tmp_version}" [current_project] }
set_property dsa.hpfm_file "zybo_z7_20.hpfm" [current_project]
set_property dsa.platform_state "pre_synth" [current_project]
write_dsa -force -unified zybo_z7_20.dsa
validate_dsa zybo_z7_20.dsa -verbose
