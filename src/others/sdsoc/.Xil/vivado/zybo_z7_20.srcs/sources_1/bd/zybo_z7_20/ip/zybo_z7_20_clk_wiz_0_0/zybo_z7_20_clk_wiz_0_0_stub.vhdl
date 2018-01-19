-- Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2017.2_sdx (lin64) Build 1972098 Wed Aug 23 11:34:38 MDT 2017
-- Date        : Mon Nov 20 21:05:56 2017
-- Host        : ubuntu running 64-bit unknown
-- Command     : write_vhdl -force -mode synth_stub
--               /home/digilent/work/git/Zybo-Z7-20-base-sdsoc/src/bd/zybo_z7_20/ip/zybo_z7_20_clk_wiz_0_0/zybo_z7_20_clk_wiz_0_0_stub.vhdl
-- Design      : zybo_z7_20_clk_wiz_0_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7z020clg400-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity zybo_z7_20_clk_wiz_0_0 is
  Port ( 
    clk_out1 : out STD_LOGIC;
    clk_out2 : out STD_LOGIC;
    clk_out3 : out STD_LOGIC;
    locked : out STD_LOGIC;
    clk_in1 : in STD_LOGIC
  );

end zybo_z7_20_clk_wiz_0_0;

architecture stub of zybo_z7_20_clk_wiz_0_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "clk_out1,clk_out2,clk_out3,locked,clk_in1";
begin
end;
