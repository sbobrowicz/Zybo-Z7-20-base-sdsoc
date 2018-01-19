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

#ifndef _OPTICAL_FLOW_INT_H_
#define _OPTICAL_FLOW_INT_H_

#include <iostream>
#include <stdio.h>
#include <common/xf_common.h>
#include <imgproc/xf_dense_npyr_optical_flow.hpp>
#include <helper.h>

#include "optical_flow.h"

#define OF_BYTES_PER_PIXEL	2
#define OF_HEIGHT		1080
#define OF_WIDTH		1920
#define OF_WINDOW_SIZE		10
#ifndef OF_PIX_PER_CLOCK
#define OF_PIX_PER_CLOCK	1
#endif

struct optical_flow_data {
	xf::Mat<XF_8UC1,  OF_HEIGHT, OF_WIDTH, OF_PIX_PER_CLOCK> *luma_prev;
	xf::Mat<XF_8UC1,  OF_HEIGHT, OF_WIDTH, OF_PIX_PER_CLOCK> *luma_curr;
	xf::Mat<XF_32FC1, OF_HEIGHT, OF_WIDTH, OF_PIX_PER_CLOCK> *flow_x;
	xf::Mat<XF_32FC1, OF_HEIGHT, OF_WIDTH, OF_PIX_PER_CLOCK> *flow_y;
	uint32_t in_fourcc;
	uint32_t out_fourcc;
};

#ifdef __cplusplus
extern "C" {
#endif

void optical_flow_wrap(struct optical_flow_data *ofd,
			unsigned short *frame_prev,
			unsigned short *frame_curr,
			unsigned short *frame_out,
			int height, int width, int stride);

#ifdef __cplusplus
}
#endif

#endif
