// Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2017.2_sdx (lin64) Build 1972098 Wed Aug 23 11:34:38 MDT 2017
// Date        : Fri Oct 27 14:05:59 2017
// Host        : ubuntu running 64-bit unknown
// Command     : write_verilog -force -mode synth_stub
//               /home/digilent/work/git/Zybo-Z7-20-base-sdsoc/src/bd/zybo_z7_20/ip/zybo_z7_20_xbar_1/zybo_z7_20_xbar_1_stub.v
// Design      : zybo_z7_20_xbar_1
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z020clg400-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "axi_crossbar_v2_1_13_axi_crossbar,Vivado 2017.2_sdx" *)
module zybo_z7_20_xbar_1(aclk, aresetn, s_axi_awaddr, s_axi_awprot, 
  s_axi_awvalid, s_axi_awready, s_axi_wdata, s_axi_wstrb, s_axi_wvalid, s_axi_wready, 
  s_axi_bresp, s_axi_bvalid, s_axi_bready, s_axi_araddr, s_axi_arprot, s_axi_arvalid, 
  s_axi_arready, s_axi_rdata, s_axi_rresp, s_axi_rvalid, s_axi_rready, m_axi_awaddr, 
  m_axi_awprot, m_axi_awvalid, m_axi_awready, m_axi_wdata, m_axi_wstrb, m_axi_wvalid, 
  m_axi_wready, m_axi_bresp, m_axi_bvalid, m_axi_bready, m_axi_araddr, m_axi_arprot, 
  m_axi_arvalid, m_axi_arready, m_axi_rdata, m_axi_rresp, m_axi_rvalid, m_axi_rready)
/* synthesis syn_black_box black_box_pad_pin="aclk,aresetn,s_axi_awaddr[31:0],s_axi_awprot[2:0],s_axi_awvalid[0:0],s_axi_awready[0:0],s_axi_wdata[31:0],s_axi_wstrb[3:0],s_axi_wvalid[0:0],s_axi_wready[0:0],s_axi_bresp[1:0],s_axi_bvalid[0:0],s_axi_bready[0:0],s_axi_araddr[31:0],s_axi_arprot[2:0],s_axi_arvalid[0:0],s_axi_arready[0:0],s_axi_rdata[31:0],s_axi_rresp[1:0],s_axi_rvalid[0:0],s_axi_rready[0:0],m_axi_awaddr[351:0],m_axi_awprot[32:0],m_axi_awvalid[10:0],m_axi_awready[10:0],m_axi_wdata[351:0],m_axi_wstrb[43:0],m_axi_wvalid[10:0],m_axi_wready[10:0],m_axi_bresp[21:0],m_axi_bvalid[10:0],m_axi_bready[10:0],m_axi_araddr[351:0],m_axi_arprot[32:0],m_axi_arvalid[10:0],m_axi_arready[10:0],m_axi_rdata[351:0],m_axi_rresp[21:0],m_axi_rvalid[10:0],m_axi_rready[10:0]" */;
  input aclk;
  input aresetn;
  input [31:0]s_axi_awaddr;
  input [2:0]s_axi_awprot;
  input [0:0]s_axi_awvalid;
  output [0:0]s_axi_awready;
  input [31:0]s_axi_wdata;
  input [3:0]s_axi_wstrb;
  input [0:0]s_axi_wvalid;
  output [0:0]s_axi_wready;
  output [1:0]s_axi_bresp;
  output [0:0]s_axi_bvalid;
  input [0:0]s_axi_bready;
  input [31:0]s_axi_araddr;
  input [2:0]s_axi_arprot;
  input [0:0]s_axi_arvalid;
  output [0:0]s_axi_arready;
  output [31:0]s_axi_rdata;
  output [1:0]s_axi_rresp;
  output [0:0]s_axi_rvalid;
  input [0:0]s_axi_rready;
  output [351:0]m_axi_awaddr;
  output [32:0]m_axi_awprot;
  output [10:0]m_axi_awvalid;
  input [10:0]m_axi_awready;
  output [351:0]m_axi_wdata;
  output [43:0]m_axi_wstrb;
  output [10:0]m_axi_wvalid;
  input [10:0]m_axi_wready;
  input [21:0]m_axi_bresp;
  input [10:0]m_axi_bvalid;
  output [10:0]m_axi_bready;
  output [351:0]m_axi_araddr;
  output [32:0]m_axi_arprot;
  output [10:0]m_axi_arvalid;
  input [10:0]m_axi_arready;
  input [351:0]m_axi_rdata;
  input [21:0]m_axi_rresp;
  input [10:0]m_axi_rvalid;
  output [10:0]m_axi_rready;
endmodule
