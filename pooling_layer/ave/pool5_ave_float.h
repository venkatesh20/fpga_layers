#ifndef __POOL5_AVE_FLOAT_H__
#define __POOL5_AVE_FLOAT_H__

#include <math.h>
#include <limits.h>

#define NUM_MASK_ROWS 3
#define NUM_MASK_COLS 3
#define STRIDE 2
#define IDX2C(i,j,ld) (((j)*(ld))+(i))
#define IWIDTH 6
#define IHEIGHT 6
#define OWIDTH 3
#define OHEIGHT 3

// Prototype of top level function for C-synthesis
void pool5_ave_float(float *in,float *out);

#endif // __POOL5_AVE_FLOAT_H__ not defined
