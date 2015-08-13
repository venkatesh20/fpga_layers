#include "lrn2_ac_float.h"
#include <math.h>
#include <stdio.h>

void lrn2_ac_layer(float *input, float *output) {

  #pragma HLS INTERFACE m_axi port=input offset=slave bundle=lrn2_gmem
  #pragma HLS INTERFACE m_axi port=output offset=slave bundle=lrn2_gmem
  #pragma HLS INTERFACE s_axilite port=input bundle=lrn2_control
  #pragma HLS INTERFACE s_axilite port=output bundle=lrn2_control
  #pragma HLS INTERFACE s_axilite port=return bundle=lrn2_control
 

  float alpha=ALPHA;
  float beta=BETA;
  int size=LOCAL_SIZE;

  float bufin[ISIZE];
  float bufout[OSIZE];

  memcpy(bufin, input, ISIZE*sizeof(float));

  int n,c,h,w,i;
  BLOB_LOOP: for (n = 0; n < NUM_OF_BOTTOM_BLOBS; ++n) {
    CHANNELS_LOOP: for (c = 0; c < NUM_CHANNELS; ++c) {
      IHEIGHT_LOOP: for (h = 0; h < IHEIGHT; ++h) {
        IWIDTH_LOOP: for (w = 0; w < IWIDTH; ++w) {
          int c_start = c - (size - 1) / 2;
          int c_end = c_start + size < NUM_CHANNELS ? c_start + size : NUM_CHANNELS ;
          c_start = c_start > 0 ? c_start : 0;
          float scale = 1.0;
          SCALING_LOOP: for (i = c_start; i < c_end; ++i) {
          #pragma HLS loop_tripcount min=0 max=5
          #pragma HLS PIPELINE
            float value = bufin[((n * NUM_CHANNELS + i) * IHEIGHT + h) * IWIDTH + w];
            scale += value * value * alpha / size;
          }
          bufout[((n * NUM_CHANNELS + c) * OHEIGHT + h) * OWIDTH + w] =
          bufin[((n * NUM_CHANNELS + c) * IHEIGHT + h) * IWIDTH+ w]/exp(beta*log(scale));
        }
      }
    }
  }
  memcpy(output, bufout, OSIZE*sizeof(float));
}
