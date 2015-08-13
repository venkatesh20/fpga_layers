/*
 * relu_1_kernel.c
 *
 *  Created on: Feb 26, 2015
 *      Author: Ag
 */
#include <stdio.h>

#define NO_NODES 3025 



void relu_layer(float in_array[NO_NODES], float out_array[NO_NODES])
{
#pragma HLS INTERFACE m_axi port=in_array offset=slave bundle=gmem         
#pragma HLS INTERFACE m_axi port=out_array offset=slave bundle=gmem     
#pragma HLS INTERFACE s_axilite port=in_array bundle=control                 
#pragma HLS INTERFACE s_axilite port=out_array bundle=control             
#pragma HLS INTERFACE s_axilite port=return bundle=control              
	//****************************************************************************************************
	// This logic will take in input from all input neurons
	// consider their sum, will evaluate against zero and give output.
	//****************************************************************************************************
		//-----------------------------------------------------------------------------------------------------	
		for (int i = 0; i < NO_NODES; i++)
		{
			#pragma HLS PIPELINE
		  	//==================================================================
		 	float temp = in_array[i];
		  	if (temp >= 0)  // If out_array[i] is positive, then it remains the same else it is converted to zero. !!!!
		  	{
		  		out_array[i] = temp; 
		  	}
		  	else
		  	{
		  		out_array[i] = 0;
		  	}
        //printf("temp is %f, out is %f\n", temp, out_array[i]);
		  //==================================================================
		} // 	for (int i = 0; i < NO_OUT_NODES; i++)
	//-----------------------------------------------------------------------------------------------------
} // void relu_layer(float in_array[NO_NODES], float out_array[NO_NODES])

