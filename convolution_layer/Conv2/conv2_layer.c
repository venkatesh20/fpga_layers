#include <string.h>
#include "Conv.h"

#define DATA_SIZE_KERN      (NUM_DATA_ROWS/*+(2*PAD)*/) * (NUM_DATA_COLS/*+(2*PAD)*/)
#define FILTER_SIZE_KERN    NUM_MASK_ROWS * NUM_MASK_COLS
#define OUTPUT_SIZE_KERN    NUM_OUT_ROWS * NUM_OUT_COLS

void conv2_layer(float *a, float *b, float *output)
{
#pragma HLS INTERFACE m_axi port=a offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=b offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=output offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=a bundle=control
#pragma HLS INTERFACE s_axilite port=b bundle=control
#pragma HLS INTERFACE s_axilite port=output bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  float bufa[DATA_SIZE_KERN];
  float bufb[FILTER_SIZE_KERN];
  float bufc[OUTPUT_SIZE_KERN];

  memcpy(bufa, (float *) a, DATA_SIZE_KERN*sizeof(float));
  memcpy(bufb, (float *) b, FILTER_SIZE_KERN*sizeof(float));
  memcpy(bufc, (float *) output, OUTPUT_SIZE_KERN*sizeof(float));
  int window_start_idx = 0;
  int out_idx = 0;
  int data_idx = 0;
  int filter_idx = 0;
  int in_y = 0;
  int in_x = 0;
 OUTER_DATA_LOOP:for (int row = 0; row < NUM_OUT_ROWS; row++) {
  INNER_DATA_LOOP:for (int col = 0; col < NUM_OUT_COLS; col++) {
//#pragma HLS pipeline
      //window_start_idx = (row * (NUM_DATA_COLS) * STRIDE - PAD) + (col * STRIDE - PAD);
    out_idx = (row * (NUM_OUT_COLS)) + col;

    OUTER_MASK_LOOP:for (int filter_row = 0; filter_row < NUM_MASK_ROWS; ++filter_row) {
      INNER_MASK_LOOP:for (int filter_col = 0; filter_col < NUM_MASK_COLS; ++filter_col) {
          in_y = row * STRIDE - PAD + filter_row;
          in_x = col * STRIDE - PAD + filter_col;
          //data_idx = window_start_idx + (NUM_DATA_COLS * (filter_row)) + filter_col;
          if(in_x >= 0 && in_x < NUM_DATA_COLS && in_y >= 0 && in_y < NUM_DATA_ROWS) {
            data_idx = in_y * NUM_DATA_COLS + in_x;
            filter_idx = (filter_row * NUM_MASK_COLS) + filter_col;
            bufc[out_idx]  = bufc[out_idx]  + (bufa[data_idx] * bufb[filter_idx]);
          }
        }
      }
    }
  }


  memcpy((float *) output, bufc, OUTPUT_SIZE_KERN*sizeof(float));
  return;
}
