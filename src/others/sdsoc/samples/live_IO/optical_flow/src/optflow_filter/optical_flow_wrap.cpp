/***************************************************************************
 Copyright (c) 2016, Xilinx, Inc.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ***************************************************************************/
//#include <cmath>
//#include <linux/videodev2.h>
//#include <filter.h>

#include "optical_flow_int.h"

//#pragma SDS data mem_attribute(frame_prev:NON_CACHEABLE|PHYSICAL_CONTIGUOUS)
//#pragma SDS data mem_attribute(frame_curr:NON_CACHEABLE|PHYSICAL_CONTIGUOUS)
//#pragma SDS data copy(frame_prev[0:(stride*height)])
//#pragma SDS data copy(frame_curr[0:(stride*height)])
//#pragma SDS data access_pattern(frame_prev:SEQUENTIAL)
//#pragma SDS data access_pattern(frame_curr:SEQUENTIAL)
#pragma SDS data mem_attribute(frame_prev:NON_CACHEABLE)
#pragma SDS data mem_attribute(frame_curr:NON_CACHEABLE)
#pragma SDS data zero_copy(frame_prev[0:(stride*height)])
#pragma SDS data zero_copy(frame_curr[0:(stride*height)])
#pragma SDS data mem_attribute("luma_prev.data":NON_CACHEABLE|PHYSICAL_CONTIGUOUS)
#pragma SDS data mem_attribute("luma_curr.data":NON_CACHEABLE|PHYSICAL_CONTIGUOUS)
#pragma SDS data copy("luma_prev.data"[0:"luma_prev.size"])
#pragma SDS data copy("luma_curr.data"[0:"luma_curr.size"])
#pragma SDS data access_pattern("luma_prev.data":SEQUENTIAL)
#pragma SDS data access_pattern("luma_curr.data":SEQUENTIAL)
void read_optflow_input(
			unsigned short *frame_prev,
			unsigned short *frame_curr,
			xf::Mat<XF_8UC1, OF_HEIGHT, OF_WIDTH, OF_PIX_PER_CLOCK>& luma_prev,
			xf::Mat<XF_8UC1, OF_HEIGHT, OF_WIDTH, OF_PIX_PER_CLOCK>& luma_curr,
			int height, int width, int stride)
{
	int p = 0;
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
#pragma HLS pipeline II=1
			unsigned short yuv_prev = frame_prev[i*stride+j];
			unsigned short yuv_curr = frame_curr[i*stride+j];
			yuv_prev &= 0x00FF;
			yuv_curr &= 0x00FF;
			luma_prev.data[p]   = (unsigned char) yuv_prev;
			luma_curr.data[p++] = (unsigned char) yuv_curr;
		}
	}
}


//unsigned char getLuma (float fx, float fy, float clip_flowmag)
//{
//#pragma HLS inline
//  float rad = sqrtf (fx*fx + fy*fy); //
//
//  if (rad > clip_flowmag) rad = clip_flowmag; // clamp to MAX 10.0
//  rad /= clip_flowmag;			      // convert 0..MAX to 0.0..1.0
//  unsigned char pix = (unsigned char) (255.0f * rad);
//
//  return pix;
//}
//
//unsigned char getChroma (float f, float clip_flowmag)
//{
//#pragma HLS inline
//  if (f >   clip_flowmag ) f =  clip_flowmag; 	// clamp big positive f to  MAX
//  if (f < (-clip_flowmag)) f = -clip_flowmag; 	// clamp big negative f to -MAX
//  f /= clip_flowmag;				// convert -MAX..MAX to -1.0..1.0
//  unsigned char pix = (unsigned char) (127.0f * f + 128.0f);	// convert -1.0..1.0 to -127..127 to 1..255
//
//  return pix;
//}

#pragma SDS data mem_attribute("flowx.data":NON_CACHEABLE|PHYSICAL_CONTIGUOUS)
#pragma SDS data mem_attribute("flowy.data":NON_CACHEABLE|PHYSICAL_CONTIGUOUS)
#pragma SDS data copy("flowx.data"[0:"flowx.size"])
#pragma SDS data copy("flowy.data"[0:"flowy.size"])
#pragma SDS data access_pattern("flowx.data":SEQUENTIAL)
#pragma SDS data access_pattern("flowy.data":SEQUENTIAL)
//#pragma SDS data mem_attribute(frame_out:NON_CACHEABLE|PHYSICAL_CONTIGUOUS)
//#pragma SDS data copy(frame_out[0:(stride*height)])
//#pragma SDS data access_pattern(frame_out:SEQUENTIAL)
#pragma SDS data mem_attribute(frame_out:NON_CACHEABLE)
#pragma SDS data zero_copy(frame_out[0:(stride*height)])
void write_optflow_output(
			xf::Mat<XF_32FC1, OF_HEIGHT, OF_WIDTH, OF_PIX_PER_CLOCK> &flowx,
			xf::Mat<XF_32FC1, OF_HEIGHT, OF_WIDTH, OF_PIX_PER_CLOCK> &flowy,
			unsigned short *frame_out,
			int height, int width, int stride)
{
	//float clip_flowmag = 10.0f;

	int p = 0;
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
#pragma HLS pipeline II=1
			float fx = flowx.data[p];   // sqrt(fx^2+fy^2) max is 10.0
			float fy = flowy.data[p++]; // fx,fy ~ in the -10..10 range
			int ifx = (int)(fx * 20.0f); // ifx,ify ~ in the -200..200 range
			int ify = (int)(fy * 20.0f);

			unsigned int ufx = (unsigned int)((ifx <0)? -ifx : ifx); // ufx,ufy ~ in the 0..200 range
			unsigned int ufy = (unsigned int)((ify <0)? -ify : ify);
			unsigned int mag = ufx+ufy;
			mag = (mag>255)? 255 : mag;
			unsigned char outLuma = (unsigned char) mag;

			int cfx = (ifx < -127)? -127 : ifx;
			cfx = (cfx > 127)? 127 : cfx;
			cfx += 128;
			int cfy = (ify < -127)? -127 : ify;
			cfy = (cfy > 127)? 127 : cfy;
			cfy += 128;
			unsigned char outchroma = (unsigned char) ((j&1)? cfy : cfx);

			unsigned short yuvpix = (unsigned short)((unsigned short)outLuma | ((unsigned short)outchroma << 8));
			frame_out[i*stride+j] = yuvpix;
		}
	}
}

void optical_flow_wrap(struct optical_flow_data *ofd,
			unsigned short *frame_prev,
			unsigned short *frame_curr,
			unsigned short *frame_out,
			int height, int width, int stride)
{
	//struct optical_flow_data *ofd = (struct optical_flow_data *) fs->data;
	//int pcnt = height*width;

	read_optflow_input(frame_prev, frame_curr, *ofd->luma_prev, *ofd->luma_curr, height, width, stride);
	
	xf::DenseNonPyrLKOpticalFlow<OF_WINDOW_SIZE, XF_8UC1, OF_HEIGHT, OF_WIDTH, OF_PIX_PER_CLOCK>(*ofd->luma_prev, *ofd->luma_curr, *ofd->flow_x, *ofd->flow_y);

	write_optflow_output(*ofd->flow_x, *ofd->flow_y, frame_out, height, width, stride);

	return;
}
