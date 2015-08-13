#include "pool2_max_layer.h"
#include <string.h>
#include <float.h>
#include <stdio.h>

void pool2_max_layer(float *in, float *out) {
  #pragma HLS INTERFACE m_axi port=in offset=slave bundle=pool_gmem
  #pragma HLS INTERFACE m_axi port=out offset=slave bundle=pool_gmem
  #pragma HLS INTERFACE s_axilite port=in bundle=pool_control
  #pragma HLS INTERFACE s_axilite port=out bundle=pool_control
  #pragma HLS INTERFACE s_axilite port=return bundle=pool_control

  float inbuf[IWIDTH*IHEIGHT], obuf[OWIDTH*OHEIGHT];

  //memcpy(inbuf, (float *)in, IWIDTH*IHEIGHT); 

  int i;
  for (i=0; i<IWIDTH*IHEIGHT; i++)
      inbuf[i]=in[i];
 
  float m;
  int idx;
  int count = 0;
  int col, row, pcol, prow;

OUTER_MAX_DATA_LOOP:for (row = 0; row < IHEIGHT - NUM_MASK_ROWS + 1; row += STRIDE) {

    #pragma HLS PIPELINE
    #pragma HLS loop_tripcount min=26 max=26
    INNER_MAX_DATA_LOOP:for (col = 0; col < IWIDTH - NUM_MASK_COLS + 1 ; col += STRIDE) {
           #pragma HLS loop_tripcount min=26 max=26
                            m = -FLT_MAX;
                            idx = -1;
         OUTER_MAX_MASK_LOOP:for (prow = 0; (prow < NUM_MASK_ROWS && row + prow < IHEIGHT); ++prow) {
               #pragma HLS loop_tripcount min=3 max=3
              INNER_MAX_MASK_LOOP:for (pcol = 0; (pcol < NUM_MASK_COLS && col + pcol < IWIDTH); ++pcol) {
                    #pragma HLS loop_tripcount min=3 max=3
                                      //printf("INBUF: %f\n", inbuf[IDX2C(row+prow, col+pcol, IHEIGHT)]);  
                                     if (inbuf[IDX2C(col + pcol, row + prow, IWIDTH)] > m) {
                                      idx = IDX2C(col + pcol, row + prow, IWIDTH);
                                       m = inbuf[idx];
                                         }
                                   }
                               }
          obuf[count] = m;
          count++;
      }
  }  
   //memcpy((float*)out, obuf, OWIDTH*OHEIGHT);
   for (i=0; i<OWIDTH*OHEIGHT; i++)
     out[i]=obuf[i];
}



