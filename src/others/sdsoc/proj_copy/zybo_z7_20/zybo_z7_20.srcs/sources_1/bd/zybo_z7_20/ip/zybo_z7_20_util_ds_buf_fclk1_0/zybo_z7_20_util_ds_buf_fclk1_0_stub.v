// Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2017.2_sdx (lin64) Build 1972098 Wed Aug 23 11:34:38 MDT 2017
// Date        : Mon Nov 20 21:05:53 2017
// Host        : ubuntu running 64-bit unknown
// Command     : write_verilog -force -mode synth_stub
//               /home/digilent/work/git/Zybo-Z7-20-base-sdsoc/src/bd/zybo_z7_20/ip/zybo_z7_20_util_ds_buf_fclk1_0/zybo_z7_20_util_ds_buf_fclk1_0_stub.v
// Design      : zybo_z7_20_util_ds_buf_fclk1_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z020clg400-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "util_ds_buf,Vivado 2017.2_sdx" *)
module zybo_z7_20_util_ds_buf_fclk1_0(BUFG_I, BUFG_O)
/* synthesis syn_black_box black_box_pad_pin="BUFG_I[0:0],BUFG_O[0:0]" */;
  input [0:0]BUFG_I;
  output [0:0]BUFG_O;
endmodule
