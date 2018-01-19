# Zybo Z7-20 Base SDSoC Design
Created for Vivado 2017.2
Targets SDSoC 2017.2

This project is an internal project used by Digilent for the Zybo Z7-20 
SDSoC/reVISION platform. It contains the Vivado project, as well as an SDSoC
platform generator project and Petalinux project in the ./src/others folder. This
project can be used by advanced users who wish to make modifications to the SDSoC
platform distributed by Digilent. Digilent does not provide documentation for doing 
this, so the document titled UG1146 from Xilinx should be followed instead.

## Known Issues

1. Typically the processing system IP core will infer a BUFG on the FCLK signals. For some reason, this is occuring for FCLK 0 only.
   FCLK2 seems to be getting a BUFG added during implementation, so it doesn't cause any issues for that net, but FCLK 1 was being
   routed as a normal signal (not on the global clock network). This caused insanely long build times and failure to meet timing. The
   current work around is to manually insert a BUFG on FCLK1 using a util_ds_buf IP core.

2. Audio is not tested

## Questions for Xilinx

1. Is it possible to get initramdisk rootfs mode to work when the ramdisk size is greater than 128 MB?
