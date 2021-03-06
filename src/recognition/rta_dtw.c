/**
 * @file rta_dtw.c
 * @author Baptiste Caramiaux
 * @date 08/10/08
 *
 * @brief Compute a Dynamic Time Warping
 *
 * @copyright
 * Copyright (C) 2008 - 2009 by IRCAM-Centre Georges Pompidou, Paris, France.
 * All rights reserved.
 * 
 * License (BSD 3-clause)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <math.h>

#include "rta_math.h"
#include "rta_dtw.h"

#define INF HUGE_VAL



/*
 * Construct Score Matrix
 *
 * Score matrix is based on euclidean distance
 * Signals must have the same number of observations
 */
static int
prepare_score_matrix(float * A, int m_A, float * B, int m_B, int n, float * out_R)
{
	int		i, j, k;
	float	max_d	= 0.;
	float	tmp		= 0.;
	
	//Compute euclidean distance between the i-th first signal observation
	//and the j-th second signal observation for all i
	for ( i = 0; i < m_A; i++ )
		for ( j = 0; j < m_B; j++ )
		{
			out_R[i * m_B + j] = 0.0;
			
			for ( k = 0; k < n; k++ )
				out_R[i * m_B + j] += ( A[i * n + k] - B[j * n + k] ) * ( A[i * n + k] - B[j * n + k] );
			
			if ( fabs(out_R[i * m_B + j]) > max_d)
				max_d = fabs(out_R[i * m_B + j]);
		}
	
	//Normalization
	for ( i = 0; i < m_A; i++ )
		for ( j = 0; j < m_B; j++ )
		{
			tmp = (out_R[i * m_B + j] / max_d);
			out_R[i * m_B + j] = tmp;
		}	
	
	return 0;
}



/*
 * Dynamic Programming Routine
 * 
 * Copyright (c) 2003-05 Dan Ellis <dpwe@ee.columbia.edu>
 * released under GPL - see file COPYRIGHT
 */
static int
dpcore(float * pM, int rows, int cols, int * pC, int crows, int ccols, float * pD, int * pP)
{
	
	
	//int rows, cols, 
	int i, j, k, tb;
	
	float d1, d2, v;
	float *costs;
	int *steps;
	int ncosts;
	int ii;
	
	ncosts = crows;
	costs = (float *)malloc( ncosts * sizeof(float));
	steps = (int *)malloc( ncosts * 2 * sizeof(int));
	
	for (ii = 0; ii < ncosts; ++ii) 
	{
		steps[2*ii] = (int)(pC[ii * ccols]);
		steps[2*ii+1] = (int)(pC[ii * ccols + 1]);
		costs[ii] = (float)(pC[ii * ccols + 2]);
	}
	
	
	
	/* do dp */
	v = 0;	
	tb = 1;	/* value to use for 0,0 */
	
	for (j = 0; j < cols; ++j) 
	{
		for (i = 0; i < rows; ++i) 
		{
			d1 = pM[i * cols + j];
			
			for (k = 0; k < ncosts; ++k) 
			{
				if ( i >= steps[2*k] && j >= steps[2*k+1] ) 
				{
					d2 = costs[k]*d1 + pD[(i-steps[2*k]) * cols + (j-steps[2*k+1])];
					
					if (d2 < v) {
						v = d2;
						tb = k;
					}
				}
			}
			
			pD[i * cols + j] = v;
			pP[i * cols + j] = tb;
			v = INF;
		}
	}
	free((void *)costs);
	free((void *)steps);
	
	return 0;
}





/*
 *
 * dpfast fill index tables p and q using the dpcore function
 * dpcore function is copyrighted by Dan Ellis, under GPL licence
 *
 */
static int
dpfast(float * SM, int m, int n, int T1, int T2, int * p, int * q, int * length)
{
	
	
	int i	= m-1;
	int	j	= n-1;
	int cpt = 0;
	int k, tb;
	float * pD;
	int * pP;

	/*
	 * C is the step and weight matrix
	 * two first columns : first and second dimension steps
	 * thirs columns : the corresponding weight at (i,j)
	 */
	int * C = (int *)malloc( 9 * sizeof(int));
	C[0] = 1; C[1] = 1; C[2] = 1;
	C[3] = 1; C[4] = 0; C[5] = 1;
	C[6] = 0; C[7] = 1; C[8] = 1;
	
	//cost matrix
	pD	= (float *)malloc( m * n * sizeof(float));
	
	//index matrix
	pP	= (int *)malloc( m * n * sizeof(int));
	
	//initialization
	for( k = 0; k < m * n; k++)
	{
	    pD[k] = 0.0; pP[k] = 0;
	}
	
	
	//computation of the cost matrix
	dpcore( SM, m, n, C, 3, 3, pD, pP);

	
	//build index tables corresponding to the
	//first and the second signal
	p[0] = i;
	q[0] = j;
	
	while ( i!= 0 || j!= 0)
	{
		
		tb = pP[i * n + j];
			
		i -= C[tb * 3];
		j -= C[tb * 3 + 1];
		
		cpt+=1;
		
		p[cpt] = i;
		q[cpt] = j;
		
	}
	
	*length = cpt + 1;

	return 0;
}



/*
 * rta_dtw : main function
 *	see rta_dtw.h
 */
int
rta_dtw(float * left_ptr, int left_m, int left_n, float * right_ptr, int right_m, int right_n,
		float * output_p, float * output_q, float * output_A, float * output_B, float * output_SM, int * length)
{
	
	int i;
	int j;
//	int cptp;
//	int cptq;
//	int indp1;
//	int indq1;
	int * p;
	int * q;

	//Fill cost (or score) matrix : SM
	float * SM	= (float *)malloc( left_m * right_m * sizeof(float));
	prepare_score_matrix( left_ptr, left_m, right_ptr, right_m, left_n, SM);
	
	
	//Compute index tables p,q using dynamic programming
	p = (int *)malloc( (left_m + right_m + 1) * sizeof(int));
	q = (int *)malloc( (left_m + right_m + 1) * sizeof(int));
	dpfast(SM, left_m, right_m, 1, 1, p, q, length);

	
	//Build outputs
	for (i = 0; i < *length; i++)
	{
			output_p[i] = p[(*length - 1) - i];
			output_q[i] = q[(*length - 1) - i];
	}
	
	//Aligned data
//	indp1 = (int)output_p[0];
//	indq1 = (int)output_q[0];
//	cptp = 0;
//	cptq = 0;
	for (i = 0; i < *length; i++)
	{
		for (j = 0; j < left_n; j++)
		{
			output_A[i * left_n + j]  = left_ptr[(int)output_p[i]  * left_n + j];
			output_B[i * right_n + j] = right_ptr[(int)output_q[i] * right_n + j];
		}
	}
	
	for (i = 0; i < left_m; i++)
		for (j = 0; j < right_m; j++)
			output_SM[i * right_m + j] = SM[i * right_m + j];
	
	return 0;
}
