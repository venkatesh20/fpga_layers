#ifndef LRN1_WC_FLOAT_H_INCLUDED
#define LRN1_WC_FLOAT_H_INCLUDED

#include <math.h>
#include <limits.h>

#define IWIDTH 55
#define IHEIGHT 55
#define OWIDTH 55
#define OHEIGHT 55

#define NUM_OF_BOTTOM_BLOBS 10 
#define NUM_CHANNELS 40 
#define ALPHA 0.0001
#define BETA 0.75
#define LOCAL_SIZE 5

#define ISIZE NUM_OF_BOTTOM_BLOBS*NUM_CHANNELS*IWIDTH*IHEIGHT
#define OSIZE NUM_OF_BOTTOM_BLOBS*NUM_CHANNELS*OWIDTH*OHEIGHT

void lrn1_wc_float(float *input, float *output);

#endif // LRN1_WC_FLOAT_H_INCLUDED
