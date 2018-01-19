#SDSoC Platform hardware metadata creation script

#First run source -notrace <SDSoC Install dir>/scripts/vivado/sdsoc_pfm.tcl

set pfm [sdsoc::create_pfm zybo_z7_20.hpfm]
sdsoc::pfm_name $pfm "digilentinc.com" "xd" "zybo_z7_20" "1.0"
sdsoc::pfm_description $pfm "Zybo Z7-20 Development board with support for HDMI input and HDMI output"
sdsoc::pfm_clock $pfm FCLK_CLK0 processing_system7_0 0 true rst_ps7_fclk0
sdsoc::pfm_clock $pfm BUFG_O util_ds_buf_fclk1 1 false rst_ps7_fclk1
sdsoc::pfm_clock $pfm FCLK_CLK3 processing_system7_0 2 false rst_ps7_fclk3
sdsoc::pfm_clock $pfm clk_out2 clk_wiz_0 3 false rst_clkwiz0_clk2
sdsoc::pfm_clock $pfm clk_out3 clk_wiz_0 4 false rst_clkwiz0_clk3
sdsoc::pfm_axi_port $pfm M_AXI_GP1 processing_system7_0 M_AXI_GP
sdsoc::pfm_axi_port $pfm S_AXI_ACP processing_system7_0 S_AXI_ACP
sdsoc::pfm_axi_port $pfm S_AXI_HP2 processing_system7_0 S_AXI_HP
sdsoc::pfm_axi_port $pfm S_AXI_HP3 processing_system7_0 S_AXI_HP
for {set i 6} {$i < 16} {incr i} {
sdsoc::pfm_irq $pfm In$i xlconcat_0
}
sdsoc::generate_hw_pfm $pfm

