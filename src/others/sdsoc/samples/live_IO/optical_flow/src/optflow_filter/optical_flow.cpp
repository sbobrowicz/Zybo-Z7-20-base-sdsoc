/******************************************************************************
 *
 * (c) Copyright 2012-2016 Xilinx, Inc. All rights reserved.
 *
 * This file contains confidential and proprietary information of Xilinx, Inc.
 * and is protected under U.S. and international copyright and other
 * intellectual property laws.
 *
 * DISCLAIMER
 * This disclaimer is not a license and does not grant any rights to the
 * materials distributed herewith. Except as otherwise provided in a valid
 * license issued to you by Xilinx, and to the maximum extent permitted by
 * applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
 * FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
 * IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
 * MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
 * and (2) Xilinx shall not be liable (whether in contract or tort, including
 * negligence, or under any other theory of liability) for any loss or damage
 * of any kind or nature related to, arising under or in connection with these
 * materials, including for any direct, or any indirect, special, incidental,
 * or consequential loss or damage (including loss of data, profits, goodwill,
 * or any type of loss or damage suffered as a result of any action brought by
 * a third party) even if such damage or loss was reasonably foreseeable or
 * Xilinx had been advised of the possibility of the same.
 *
 * CRITICAL APPLICATIONS
 * Xilinx products are not designed or intended to be fail-safe, or for use in
 * any application requiring fail-safe performance, such as life-support or
 * safety devices or systems, Class III medical devices, nuclear facilities,
 * applications related to the deployment of airbags, or any other applications
 * that could lead to death, personal injury, or severe property or
 * environmental damage (individually and collectively, "Critical
 * Applications"). Customer assumes the sole risk and liability of any use of
 * Xilinx products in Critical Applications, subject only to applicable laws
 * and regulations governing limitations on product liability.
 *
 * THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
 * AT ALL TIMES.
 *
 *******************************************************************************/

//#include <filter.h>

#include "optical_flow_int.h"

#ifdef REPORT_ELAPSED_TIME
#include "sds_lib.h"
unsigned long long clock_start, clock_end, tks;
unsigned long long tps;
#define TIME_STAMP_INIT  { clock_start = sds_clock_counter(); }
#define TIME_STAMP  { clock_end = sds_clock_counter(); tks=clock_end-clock_start; printf("%dth frame: %5.1f ms %5.1f fps\n", fct, 1000.0f*(float)tks/(float)tps, (float)tps/(float)tks ); }
#endif

struct optical_flow_data *ofd;

int optical_flow_init(const struct filter_init_data *fid) 
{

	/* Check that dimensions are what we expect*/
	if (fid->in_height  > OF_HEIGHT    ||
		fid->in_width   > OF_WIDTH ||
		fid->out_height > OF_HEIGHT    ||
		fid->out_width  > OF_WIDTH       ){
		fprintf(stderr, "Input height,width %lu,%lu: must be <= %d,%d\n",
			fid->in_height, fid->in_width, OF_HEIGHT, OF_WIDTH);
		fprintf(stderr, "Output height,width %lu,%lu: must be <= %d,%d\n",
			fid->out_height, fid->out_width, OF_HEIGHT, OF_WIDTH);
		return -1;
	}

	ofd = (struct optical_flow_data *) malloc(sizeof(struct optical_flow_data));
	if (ofd == NULL) {
		return -1;
	}

	ofd->luma_prev = new xf::Mat<XF_8UC1,  OF_HEIGHT, OF_WIDTH, OF_PIX_PER_CLOCK> (fid->in_height, fid->in_width);
	ofd->luma_curr = new xf::Mat<XF_8UC1,  OF_HEIGHT, OF_WIDTH, OF_PIX_PER_CLOCK> (fid->in_height, fid->in_width);
	ofd->flow_x = new xf::Mat<XF_32FC1, OF_HEIGHT, OF_WIDTH, OF_PIX_PER_CLOCK> (fid->in_height, fid->in_width);
	ofd->flow_y = new xf::Mat<XF_32FC1, OF_HEIGHT, OF_WIDTH, OF_PIX_PER_CLOCK> (fid->in_height, fid->in_width);
	ofd->in_fourcc = fid->in_fourcc;
	ofd->out_fourcc = fid->out_fourcc;

	return 0;
}

//static void of_sw_dummy(void) {}

void optical_flow_func(unsigned short *frm_prev, 
					  unsigned short *frm_curr, 
					  unsigned short *frm_out,
					  int height, int width, int stride)
{
	static int fct = 0;

#ifdef REPORT_ELAPSED_TIME
	tps = sds_clock_frequency();
#endif

	//switch (fs->mode) {
#ifdef WITH_SDSOC
	//case FILTER_MODE_HW:
#ifdef REPORT_ELAPSED_TIME
		if ((++fct % 60) ==0) // calculate time every 60 frames (1 sec @ 60 hz)
		{
			TIME_STAMP_INIT
		}
#endif

		optical_flow_wrap(ofd,
			frm_prev,
			frm_curr,
			frm_out,
			height, width, stride);

#ifdef REPORT_ELAPSED_TIME
		if ((fct % 60) ==0) // print accelerator elapsed time
		{
			TIME_STAMP
		}
#endif
		//break;
#endif
	//case FILTER_MODE_SW:
	//default:
	//	of_sw_dummy();
	//	break;
	//}
}

//static struct filter_ops ops;
//
//const static struct filter_s FS = {
//	"Optical Flow",
//	"",
//	"",
//	NULL,
//	-1,
//	FILTER_MODE_OFF,
//	&ops
//};
//
//struct filter_s *optical_flow_create(void)
//{
//	ops.init = optical_flow_init;
//	ops.func2 = optical_flow_func2;
//
//	struct filter_s *fs = (struct filter_s *) (malloc(sizeof *fs));
//	*fs = FS;
//
//	return fs;
//}
