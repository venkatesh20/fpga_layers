#include "lrn2_wc_float.h"
#include <math.h>
#include <stdio.h>

void lrn2_wc_float(float *input, float *output) {
    #pragma HLS INTERFACE m_axi port=input offset=slave bundle=lrn2_gmem
    #pragma HLS INTERFACE m_axi port=output offset=slave bundle=lrn2_gmem
    #pragma HLS INTERFACE s_axilite port=input bundle=lrn2_control
    #pragma HLS INTERFACE s_axilite port=output bundle=lrn2_control
    #pragma HLS INTERFACE s_axilite port=return bundle=lrn2_control

    float alpha=ALPHA;
    float beta=BETA;
    int size=LOCAL_SIZE;

    int n,c,h,w,nh, nw;
  // ((n * NUM_CHANNELS + c) * H + h) * W + w.
  BLOB_LOOP: for (n = 0; n < NUM_OF_BOTTOM_BLOBS; ++n) {
       #pragma HLS loop_tripcount min=0 max=5
      CHANNELS_LOOP: for (c = 0; c < NUM_CHANNELS; ++c) {
        #pragma HLS loop_tripcount min=0 max=5
        IHEIGHT_LOOP: for (h = 0; h < IHEIGHT; ++h) {
           #pragma HLS loop_tripcount min=0 max=55
          int h_start = h - (size - 1) / 2;
          int h_end = h_start + size < IHEIGHT ? h_start + size : IHEIGHT;
          h_start = h_start > 0 ? h_start : 0;
          IWIDTH_LOOP: for (w = 0; w < IWIDTH; ++w) {
             #pragma HLS loop_tripcount min=0 max=55
            float scale = 1.0;
            int w_start = w - (size - 1) / 2;
            int w_end = w_start + size < IWIDTH ? w_start + size : IWIDTH;
            w_start = w_start > 0 ? w_start : 0;
            INNER_HEIGHT_LOOP: for (nh = h_start; nh < h_end; ++nh) {
              #pragma HLS loop_tripcount min=0 max=55
              INNER_WIDTH_LOOP: for (nw = w_start; nw < w_end; ++nw) {
                 #pragma HLS loop_tripcount min=0 max=55
                // ((n * NUM_CHANNELS + c) * H + nh) * W + nw.
                float value = input[((n * NUM_CHANNELS + c) * IHEIGHT + nh) * IWIDTH + nw];
                scale += value * value * alpha / (size * size);
                // printf("T1 %f, T2 %f, SCALE %f\n", value*value, alpha/size, scale);
              }
            }
            output[((n * NUM_CHANNELS + c) * OHEIGHT + h) * OWIDTH + w] =
              input[((n * NUM_CHANNELS + c) * IHEIGHT + h) * IWIDTH + w]/exp(beta*log(scale));
          }
        }
      }
    }

}
