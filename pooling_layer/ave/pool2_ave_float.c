#include "pool2_ave_float.h"
#include <string.h>
#include <float.h>
#include <stdio.h>

void pool2_ave_float(float *in, float *out) {
  #pragma HLS INTERFACE m_axi port=in offset=slave bundle=pool_gmem
  #pragma HLS INTERFACE m_axi port=out offset=slave bundle=pool_gmem
  #pragma HLS INTERFACE s_axilite port=in bundle=pool_control
  #pragma HLS INTERFACE s_axilite port=out bundle=pool_control
  #pragma HLS INTERFACE s_axilite port=return bundle=pool_control

  float inbuf[IWIDTH*IHEIGHT];
  float obuf[OWIDTH*OHEIGHT];
  int i;
  for(i=0; i<IWIDTH*IHEIGHT;i++)
    inbuf[i]=in[i]; 
  
  
  float m = 0.0;
  int idx;
  int count = 0;
  int col, row, pcol, prow;

OUTER_AVE_DATA_LOOP:for (row = 0; row < IHEIGHT - NUM_MASK_ROWS + 1; row += STRIDE) {
    #pragma HLS PIPELINE
    #pragma HLS loop_tripcount min=26 max=26
    INNER_AVE_DATA_LOOP:for (col = 0; col < IWIDTH - NUM_MASK_COLS + 1 ; col += STRIDE) {
            #pragma HLS loop_tripcount min=26 max=26
                            idx = -1;
         OUTER_AVE_MASK_LOOP:for (prow = 0; (prow < NUM_MASK_ROWS && row + prow < IHEIGHT); ++prow) {
                #pragma HLS loop_tripcount min=3 max=3
              INNER_AVE_MASK_LOOP:for (pcol = 0; (pcol < NUM_MASK_COLS && col + pcol < IWIDTH); ++pcol) {
                              #pragma HLS loop_tripcount min=3 max=3
            	                      idx = IDX2C(col + pcol, row + prow, IWIDTH);
                                       m+= inbuf[idx];

                                   }
                               }
          obuf[count] = m/(NUM_MASK_ROWS*NUM_MASK_COLS);
          m=0;
          count++;
      }
  }
    for(i=0; i<OWIDTH*OHEIGHT; i++)
      out[i]=obuf[i];
}


