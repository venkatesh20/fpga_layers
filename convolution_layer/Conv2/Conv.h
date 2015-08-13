#ifndef __CONV_H__
#define __CONV_H__

#include <math.h>
#include <limits.h>

//using namespace std;

// Defines
#define NUM_DATA_ROWS (27)
#define NUM_DATA_COLS (27)
#define PAD           (2)
#define NUM_MASK_ROWS (5)
#define NUM_MASK_COLS (5)
#define STRIDE (1)
#define NUM_OUT_COLS (((NUM_DATA_COLS - NUM_MASK_COLS + (2*PAD) )/ (STRIDE)) + 1)
#define NUM_OUT_ROWS (((NUM_DATA_ROWS - NUM_MASK_ROWS + (2*PAD) )/ (STRIDE)) + 1)

typedef float result_t;

// Prototype of top level function for C-synthesis
//void Conv1(float *data, float* filter, float* data_out);

#endif // __CONV_H__ not defined
