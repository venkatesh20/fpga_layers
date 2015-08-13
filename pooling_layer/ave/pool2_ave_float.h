#ifndef __POOL2_AVE_FLOAT_H__
#define __POOL2_AVE_FLOAT_H__

#include <math.h>
#include <limits.h>

#define NUM_MASK_ROWS 3
#define NUM_MASK_COLS 3
#define STRIDE 2
#define IDX2C(i,j,ld) (((j)*(ld))+(i))
#define IWIDTH 19
#define IHEIGHT 19
#define OWIDTH 9
#define OHEIGHT 9

// Prototype of top level function for C-synthesis
void pool2_ave_float(float *in,float *out);

#endif // __POOL2_AVE_FLOAT_H__ not defined
