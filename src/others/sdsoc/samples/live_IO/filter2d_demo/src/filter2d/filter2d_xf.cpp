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

#ifdef WITH_SDSOC

#include "filter2d_int.h"
//#include "xf_headers.h"
#include "platform.h"

#include"hls_stream.h"
#include "ap_int.h"
#include "common/xf_common.h"
#include "common/xf_utility.h"
#include "imgproc/xf_cvt_color.hpp"
#include "imgproc/xf_cvt_color_1.hpp"
#include "imgproc/xf_custom_convolution.hpp"

#pragma SDS data mem_attribute(frm_data_in:NON_CACHEABLE|PHYSICAL_CONTIGUOUS)
#pragma SDS data mem_attribute(frm_data_out:NON_CACHEABLE|PHYSICAL_CONTIGUOUS)
#pragma SDS data zero_copy(frm_data_in[0:stride*height])
#pragma SDS data zero_copy(frm_data_out[0:stride*height])
void filter2d_xf(uint32_t *frm_data_in, uint32_t *frm_data_out,
		  int height, int width, int stride, coeff_t coeff)
{
	xf::Mat<XF_8UC4, MAX_HEIGHT, MAX_WIDTH, XF_NPPC1> src(height, width, frm_data_in);
	xf::Mat<XF_8UC4, MAX_HEIGHT, MAX_WIDTH, XF_NPPC1> dst(height, width, frm_data_out);
	xf::Mat<XF_8UC1, MAX_HEIGHT, MAX_WIDTH, XF_NPPC1> img_y(height, width);
	xf::Mat<XF_8UC1, MAX_HEIGHT/4, MAX_WIDTH, XF_NPPC1> img_u(height/4, width);
	xf::Mat<XF_8UC1, MAX_HEIGHT/4, MAX_WIDTH, XF_NPPC1> img_v(height/4, width);
	xf::Mat<XF_8UC1, MAX_HEIGHT, MAX_WIDTH, XF_NPPC1> img_yf(height, width);

	unsigned char shift = SHIFT;
//	Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC3> srcYUV(height, width);
//	Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC3> dstYUV(height, width);

//	// planes
//	Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC1> img_y(height, width);
//	Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC1> img_cr(height, width);
//	Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC1> img_cb(height, width);
//	Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC1> img_yf(height, width);

//	Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC1> img_r(height, width);
//	Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC1> img_g(height, width);
//	Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC1> img_b(height, width);
//	Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC1> img_rf(height, width);
//	Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC1> img_gf(height, width);
//	Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC1> img_bf(height, width);

	// kernel
//	Window<3,3,char> kernel;
//	for(int i=0; i<KSIZE; i++)
//		for(int j=0; j<KSIZE; j++)
//			kernel.val[i][j] = coeff[i][j];


#if __SDSCC__
	short int *filter_ptr=(short int*)sds_alloc_non_cacheable(KSIZE*KSIZE*sizeof(short int));
#else
	short int *filter_ptr=(short int*)malloc(KSIZE*KSIZE*sizeof(short int));
#endif
	for(int i = 0; i < KSIZE; i++)
	{
		for(int j = 0; j < KSIZE; j++)
		{
			filter_ptr[i*KSIZE+j] = coeff[i][j];
		}
}


#pragma HLS dataflow
#pragma HLS stream depth=20000 variable=img_u.data
#pragma HLS stream depth=20000 variable=img_v.data

	// filter
	//AXIM2Mat<MAX_STRIDE>(frm_data_in, stride, src);

	xf::rgba2iyuv(src,img_y,img_u,img_v);
	xf::filter2D<XF_BORDER_CONSTANT,KSIZE,KSIZE,XF_8UC1,XF_8UC1,MAX_HEIGHT, MAX_WIDTH,XF_NPPC1>(img_y, img_yf, filter_ptr, shift);
	xf::iyuv2rgba(img_yf,img_u,img_v,dst);

	//Mat2AXIM<MAX_STRIDE>(dst, frm_data_out, stride);
}
#endif
