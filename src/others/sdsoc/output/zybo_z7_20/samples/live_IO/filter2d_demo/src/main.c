/*
 * main.c
 *
 *  Created on: Nov 8, 2017
 *      Author: digilent
 */
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <assert.h>
//#include <stdint.h>
//#include <fcntl.h>              /* low-level i/o */
//#include <unistd.h>
//#include <errno.h>
//#include <sys/stat.h>
//#include <sys/types.h>
//#include <sys/time.h>
//#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include "drm_helper.h"
#include "platform.h"
#include "filter2d/filter2d_int.h"




void UioWrite32(uint8_t *uioMem, unsigned int offset, uint32_t data)
{
  *((uint32_t*) (uioMem+offset)) = data;
}

uint32_t UioRead32(uint8_t *uioMem, unsigned int offset)
{
  return *((uint32_t*) (uioMem+offset));
}

int main(int argc, char **argv)
{
//FBDEV stuff
    int tty_fd, mem_fd;

    uint32_t frameAddr;
    uint32_t stride;
    uint32_t hActiveIn, vActiveIn;

    int gpio_fd, dma_fd, vtc_fd, sw_fd;
    uint8_t *gpioMem, *dmaMem, *vtcMem, *swMem;

    uint32_t *fb_buf[NUM_INPUT_FB];
    uint32_t fb_addr[NUM_INPUT_FB];

    struct drm_cntrl drm;
    /*
     * This mode will need to be set to the detected input mode once we begin
     * changing the output resolution to match the input. For now it is just a
     * dummy value that is passed to drm Init function and ignored.
     */
    struct drm_mode_modeinfo dummyMode;

    /*
     * DRM setup
     */
        //Stop the terminal from being drawn on the monitor
	tty_fd = open("/dev/tty1", O_RDWR);
	ioctl(tty_fd,KDSETMODE,KD_GRAPHICS);
	close(tty_fd);

	if (drmControlInit("/dev/dri/card0",&dummyMode,&drm) != SUCCESS)
	{
		printf("drmControlInit Failed");
		return -1;
	}

    /*
     * mmap input framebuffers
     */
	mem_fd = open("/dev/mem", O_RDWR);
    if(mem_fd == -1) {
	   printf("Could not open /dev/mem\n");
	   return -1;
    }
    fb_addr[0] = FB0_ADDR;
    fb_addr[1] = FB1_ADDR;
    fb_addr[2] = FB2_ADDR;
    fb_addr[3] = FB3_ADDR;

    for (int i = 0; i < NUM_INPUT_FB; i++)
    {
    	fb_buf[i] = (uint32_t *) mmap(0, INPUT_FB_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, fb_addr[i]);
    }

    /*
     *uio initialization
     */
	sw_fd = open(UIO_SW_PATH, O_RDWR);
	gpio_fd = open(UIO_GPIO_PATH, O_RDWR);
	dma_fd = open(UIO_DMA_PATH, O_RDWR);
	vtc_fd = open(UIO_VTC_PATH, O_RDWR);

	swMem = mmap(0, UIO_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, sw_fd, (off_t)0);
	gpioMem = mmap(0, UIO_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, gpio_fd, (off_t)0);
	dmaMem = mmap(0, UIO_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dma_fd, (off_t)0);
	vtcMem = mmap(0, UIO_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, vtc_fd, (off_t)0);

    /*
     *Check if HDMI input clock locked
     */
	UioWrite32(gpioMem, 0, 1); //
	UioWrite32(gpioMem, 4, 0); //set HPD as output and assert it
	UioWrite32(gpioMem, 12, 1); //set clock locked as input

	/*
	 * Set Switches as input
	 */
	UioWrite32(swMem, 4, 0xF); //set all 4 switches as input

	int isLocked = 0;
	for(int i = 0; i < 12 && !isLocked; i++)
	{
		sleep(1);
		isLocked = UioRead32(gpioMem, 8);
	}
	if (!isLocked)
	{
		printf("HDMI input clock not detected, attach HDMI source");
		return -1;
	}

    /*
     *Enable VTC and wait for it to lock
     */
	UioWrite32(vtcMem, 0, 8); //Enable detector
	isLocked = 0;
	for(int i = 0; i < 15 && !isLocked; i++)
	{
		sleep(1);
		isLocked = UioRead32(vtcMem, 0x24) & 1;
	}
	if (!isLocked) //check if locked
	{
		printf("VTC not locked, attach a stable HDMI source");
		return -1;
	}
	hActiveIn = UioRead32(vtcMem,0x20);
	vActiveIn = (hActiveIn & 0x1FFF0000) >> 16;
	hActiveIn = hActiveIn & 0x1FFF;

    /*
     *Check if input resolution fits in output resolution
     */
	if (hActiveIn > drm.current.hdisplay || vActiveIn > drm.current.vdisplay)
	{
		printf("Input resolution greater than output resolution");
		return -1;
	}

    /*
     *Start VDMA
     */
	UioWrite32(dmaMem, 0x30, 3); //Enable S2MM channel with Circular park
	UioWrite32(dmaMem, 0xAC, FB0_ADDR); //Set the frame address to physical frame of display
	UioWrite32(dmaMem, 0x28, 0); //Set Park frame to frame 0
	UioWrite32(dmaMem, 0xA4, hActiveIn * DISPLAY_BPP); //Set horizontal active size in bytes
	UioWrite32(dmaMem, 0xA8, 0x1000000 | INPUT_FB_STRIDE); //Set stride and preserve frame delay
	UioWrite32(dmaMem, 0xA0, vActiveIn); //Set the vertical active size, starting a transfer


	printf("Running Loop...\n");
	while (1)
	{
		uint32_t sw = UioRead32(swMem, 0);
		if (sw & 1)
		{
			filter2d_xf(fb_buf[0],drm.fbMem[drm.current_fb],vActiveIn, hActiveIn, INPUT_FB_STRIDE, coeff_blur);
		}
		else
		{
			filter2d_cv(fb_buf[0],drm.fbMem[drm.current_fb],vActiveIn, hActiveIn, INPUT_FB_STRIDE, coeff_blur);
		}
	}

    return 0;
}

void exit_clean(struct drm_cntrl * drm)
{
	return;
}
