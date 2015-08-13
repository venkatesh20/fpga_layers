#ifndef LRN2_WC_FLOAT_H_INCLUDED
#define LRN2_WC_FLOAT_H_INCLUDED

#include <math.h>
#include <limits.h>

#define IWIDTH 23
#define IHEIGHT 23
#define OWIDTH 23
#define OHEIGHT 23

#define NUM_OF_BOTTOM_BLOBS 4
#define NUM_CHANNELS 5
#define ALPHA 0.0001
#define BETA 0.75
#define LOCAL_SIZE 5

#define ISIZE NUM_OF_BOTTOM_BLOBS*NUM_CHANNELS*IWIDTH*IHEIGHT
#define OSIZE NUM_OF_BOTTOM_BLOBS*NUM_CHANNELS*OWIDTH*OHEIGHT

void lrn2_wc_float(float *input, float *output);

#endif // LRN2_WC_FLOAT_H_INCLUDED
