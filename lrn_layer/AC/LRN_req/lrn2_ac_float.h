#ifndef LRN2_AC_FLOAT_H_INCLUDED
#define LRN2_AC_FLOAT_H_INCLUDED

#include <math.h>
#include <limits.h>

#define IWIDTH 27
#define IHEIGHT 27
#define OWIDTH 27
#define OHEIGHT 27

#define NUM_OF_BOTTOM_BLOBS 1 
#define NUM_CHANNELS 256
#define ALPHA 0.0001
#define BETA 0.75
#define LOCAL_SIZE 5

#define ISIZE NUM_OF_BOTTOM_BLOBS*NUM_CHANNELS*IWIDTH*IHEIGHT
#define OSIZE NUM_OF_BOTTOM_BLOBS*NUM_CHANNELS*OWIDTH*OHEIGHT

void lrn2_ac_layer(float *input, float *output);

#endif // LRN2_AC_FLOAT_H_INCLUDED
