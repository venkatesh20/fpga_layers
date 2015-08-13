#ifndef __POOL1_AVE_FLOAT_H__
#define __POOL1_AVE_FLOAT_H__

#include <math.h>
#include <limits.h>

#define NUM_MASK_ROWS 3
#define NUM_MASK_COLS 3
#define STRIDE 2
#define IDX2C(i,j,ld) (((j)*(ld))+(i))
#define IWIDTH 51
#define IHEIGHT 51
#define OWIDTH 25
#define OHEIGHT 25

// Prototype of top level function for C-synthesis
void pool1_ave_float(float *in,float *out);

#endif // __POOL1_AVE_FLOAT_H__ not defined
