/**
 * @file   rta_moments.c
 * @author Jean-Philippe.Lambert@ircam.fr
 * @date   Thu Dec 13 15:28:26 2007
 * 
 * @brief  Statistical moments functions
 * 
 * The moments are calculated over the indexes and weighted by the
 * input values (eg. the amplitudes of the spectrum regularly
 * sampled). Note that all moments (but the first) are centered. The
 * results unit is index (starting from 0).
 *
 * Copyright (C) 2007 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 */

#include "rta_moments.h"
#include "rta_math.h"

/* 1st order weighted moment over indexes: weighted mean, centroid */
rta_real_t rta_weighted_moment_1_indexes(
  rta_real_t * input_sum,
  const rta_real_t * input, const unsigned int input_size)
{
  unsigned int i;
  rta_real_t centroid = 0.;
  *input_sum = 0.;

  for(i=0; i<input_size; i++)
  {
    centroid += input[i] * i;
    *input_sum += input[i];
  }

  if(*input_sum > 0.)
  {
    centroid /= *input_sum;
  }
  else
  {
    /* flat and null input => centroid is the middle */
    centroid = (input_size - 1) * 0.5;
  }

  return centroid;
}

rta_real_t rta_weighted_moment_1_indexes_stride(
  rta_real_t * input_sum, 
  const rta_real_t * input, const int i_stride,
  const unsigned int input_size)
{
  unsigned int i;
  int is;
  rta_real_t centroid = 0.;
  *input_sum = 0.;

  for(i=0, is=0; i<input_size; i++, is+=i_stride)
  {
    centroid += input[is] * i;
    *input_sum += input[is];
  }

  if(*input_sum > 0.)
  {
    centroid /= *input_sum;
  }
  else
  {
    /* flat and null input => centroid is the middle */
    centroid = (input_size - 1) * 0.5;
  }

  return centroid;
}


/* 2nd order weighted central moment over indexes: spread, weighted variance */
/* Request: input_sum != 0. */
/* note: weighted standard deviation is sqrt(weighted variance) */
rta_real_t rta_weighted_moment_2_indexes(
  const rta_real_t * input, const unsigned int input_size,
  const rta_real_t centroid, const rta_real_t input_sum)
{
  unsigned int i;
  rta_real_t moment = 0.;
  rta_real_t tmp_diff;

  for(i=0; i<input_size; i++)
  {
    tmp_diff = i - centroid;
    moment += tmp_diff * tmp_diff * input[i];
  }

  return (moment / input_sum);
}

rta_real_t rta_weighted_moment_2_indexes_stride(
  const rta_real_t * input, const int i_stride,
  const unsigned int input_size,
  const rta_real_t centroid, const rta_real_t input_sum)
{
  unsigned int i;
  int is;
  rta_real_t moment = 0.;
  rta_real_t tmp_diff;

  for(i=0, is=0; i<input_size; i++, is+=i_stride)
  {
    tmp_diff = i - centroid;
    moment += tmp_diff * tmp_diff * input[is];
  }

  return (moment / input_sum);
}

/* 3rd order weighted central moment over indexes*/
/* Request: input_sum != 0. */
rta_real_t rta_weighted_moment_3_indexes(
  const rta_real_t * input, const unsigned int input_size,
  const rta_real_t centroid, const rta_real_t input_sum)
{
  unsigned int i;
  rta_real_t moment = 0.;
  rta_real_t tmp_diff;

  for(i = 0; i<input_size; i++)
  {
    tmp_diff = i - centroid;
    moment += tmp_diff * tmp_diff * tmp_diff * input[i];
  }

  return (moment / input_sum);
}

rta_real_t rta_weighted_moment_3_indexes_stride(
  const rta_real_t * input, const int i_stride,
  const unsigned int input_size,
  const rta_real_t centroid, const rta_real_t input_sum)
{
  unsigned int i;
  int is;
  rta_real_t moment = 0.;
  rta_real_t tmp_diff;

  for(i = 0, is=0; i<input_size; i++, is+=i_stride)
  {
    tmp_diff = i - centroid;
    moment += tmp_diff * tmp_diff * tmp_diff * input[is];
  }

  return (moment / input_sum);
}

/* 3rd order standardised weighted central moment over indexes: skewness */
/* Requests: input_sum != 0. 
             deviation != 0. */
rta_real_t rta_std_weighted_moment_3_indexes(
  const rta_real_t * input, const unsigned int input_size,
  const rta_real_t centroid, const rta_real_t input_sum,
  const rta_real_t deviation)
{
  return rta_weighted_moment_3_indexes(input, input_size, centroid, input_sum) /
    (deviation * deviation * deviation);
}

rta_real_t rta_std_weighted_moment_3_indexes_stride(
  const rta_real_t * input, const int i_stride,
  const unsigned int input_size,
  const rta_real_t centroid, const rta_real_t input_sum,
  const rta_real_t deviation)
{
  return rta_weighted_moment_3_indexes_stride(
    input, i_stride, input_size, centroid, input_sum) /
    (deviation * deviation * deviation);
}

/* 4th order weighted central moment over indexes */
/* Request: input_sum != 0. */
rta_real_t rta_weighted_moment_4_indexes(
  const rta_real_t * input, const unsigned int input_size,
  const rta_real_t centroid, const rta_real_t input_sum)
{
  unsigned int i;
  rta_real_t moment = 0.;
  rta_real_t tmp_diff;

  for(i = 0; i<input_size; i++)
  {
    tmp_diff = i - centroid;
    moment += tmp_diff * tmp_diff * tmp_diff * tmp_diff * input[i];
  }

  return (moment / input_sum);
}

rta_real_t rta_weighted_moment_4_indexes_stride(
  const rta_real_t * input, const int i_stride,
  const unsigned int input_size,
  const rta_real_t centroid, const rta_real_t input_sum)
{
  unsigned int i;
  int is;
  rta_real_t moment = 0.;
  rta_real_t tmp_diff;

  for(i = 0, is=0; i<input_size; i++, is+=i_stride)
  {
    tmp_diff = i - centroid;
    moment += tmp_diff * tmp_diff * tmp_diff * tmp_diff * input[is];
  }

  return (moment / input_sum);
}

/* 4th order standardised weighted central moment over indexes: kurtosis */
/* Requests: input_sum != 0.  
             deviation != 0. */
rta_real_t rta_std_weighted_moment_4_indexes(
  const rta_real_t * input, const unsigned int input_size,
  const rta_real_t centroid, const rta_real_t input_sum,
  const rta_real_t deviation)
{
  return rta_weighted_moment_4_indexes(input, input_size, centroid, input_sum) /
    (deviation * deviation * deviation * deviation);
}

rta_real_t rta_std_weighted_moment_4_indexes_stride(
  const rta_real_t * input, const int i_stride,
  const unsigned int input_size,
  const rta_real_t centroid, const rta_real_t input_sum,
  const rta_real_t deviation)
{
  return rta_weighted_moment_4_indexes_stride(
    input, i_stride, input_size, centroid, input_sum) /
    (deviation * deviation * deviation * deviation);
}

/* general order weighted central moment over indexes */
/* Request: input_sum != 0. */
rta_real_t rta_weighted_moment_indexes(
  const rta_real_t * input, const unsigned int input_size,
  const rta_real_t centroid, const rta_real_t input_sum,
  const rta_real_t order)
{
  unsigned int i;
  rta_real_t moment = 0.;

  for(i = 0; i<input_size; i++)
  {
    moment += rta_pow(i - centroid, order) * input[i];
  }

  return (moment / input_sum);
}

rta_real_t rta_weighted_moment_indexes_stride(
  const rta_real_t * input, const int i_stride, const unsigned int input_size,
  const rta_real_t centroid, const rta_real_t input_sum,
  const rta_real_t order)
{
  unsigned int i;
  int is;
  rta_real_t moment = 0.;

  for(i = 0, is=0; i<input_size; i++, is+=i_stride)
  {
    moment += rta_pow(i - centroid, order) * input[is];
  }

  return (moment / input_sum);
}

/* general order standardised weighted central moment over indexes */
/* Requests: input_sum != 0. 
             deviation != 0. */
rta_real_t rta_std_weighted_moment_indexes(
  const rta_real_t * input, const unsigned int input_size,
  const rta_real_t centroid, const rta_real_t input_sum,
  const rta_real_t deviation,
  const rta_real_t order)
{
  return rta_weighted_moment_indexes(
    input, input_size, centroid, input_sum, order) /
    rta_pow(deviation, order);
}

rta_real_t rta_std_weighted_moment_indexes_stride(
  const rta_real_t * input, const int i_stride, const unsigned int input_size,
  const rta_real_t centroid, const rta_real_t input_sum,
  const rta_real_t deviation,
  const rta_real_t order)
{
  return rta_weighted_moment_indexes_stride(
    input, i_stride, input_size, centroid, input_sum, order) /
    rta_pow(deviation, order);
}
