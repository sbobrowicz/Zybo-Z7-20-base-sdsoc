
========================================
Project Source
========================================
**WARNING--Not all changes made to the the petalinux project are captured in this document yet, RO team!

This petalinux project was created from the petalinux project found here:

https://github.com/Digilent/Petalinux-Zybo-Z7-20

The release point used is Petalinux-Zybo-Z7-20-2017.2-1.

========================================
Changes made for SDSoC/reVISION support
========================================

The steps found in the Linux Boot Files section of UG1146 (page 31) were followed to make the 
Linux system suitable for an SDSoC platform. In addition to these changes, the following changes were
also made:

1) Instead of modifying the bootargs in the petalinux-config menu, the system-user.dtsi file was modified to include "quiet".

2)Based on references in the ZCU102 revision platform, Instead of the xlnk device tree node cited in ug1146, the following was
  used (which puts the node in amba):

&amba {
	xlnk {
		compatible = "xlnx,xlnk-1.0";
	};
};

3) Added the following packages using petalinux-config -c rootfs

	misc->packagegroup-petalinux-opencv
	misc->gdb->gdbserver
	console->utils->file->file
	console->utils->strace->strace
	misc->valgrind
	
4) Manually added all new Digilent apps. This is because they have not been committed yet to the upstream project, but will
   be useful to sdsoc users. In future releases of this project, this step won't be necessary, because the upstream petalinux project
   will already include these apps by default.

   Also, enabled several in petalinux-config -c rootfs.

5) Modified sw,btn, and led device tree nodes to load uio driver. Again, similar to #4, this should not be necessary once it gets fixed
   upstream.

6) Deleted pre-built folder to save space

7) Including opencv and the other rootfs libraries causes the ramdisk size to increase to >128MB. Booting with a ramdisk this large 
   is not currently working, so we had to switch to SD rootfs. This has other benefits, including additional available system memory,
   persistent modifications, and increased system performance in some scenarios. The main cost of this is added complexity to the SD
   card preparation, because the image has to be flashed onto the SD card. 

   The idea that this issue is caused by the rootfs being larger than 128MB compressed is speculation, another possibility is that 
   this is being caused by the existence of some hard links present in the opencv packages that are incompatible with the cpio 
   compression used with the initramdisk. 

   That was the explanation, the how is as follows: used petalinux-config to change Root Filesystem type to "SD Card". Then modified
   the bootargs in system-user.dtsi to add the following: 

   root=/dev/mmcblk0p2 rw rootwait

   Finally, after the petalinux-build, the following was used to copy the rootfs to the second partition of the SD card (replacing sdX 
   as needed):

   sudo -s
   umount /dev/sdX2
   dd if=images/linux/rootfs.ext4 of=/dev/sdX2
   sync
   resize2fs /dev/sdX2
   sync

8)



