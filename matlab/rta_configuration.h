/**
 * @file   rta_configuration.h
 * @author Jean-Philippe.Lambert@ircam.fr
 * @date   Thu May 15 21:58:21 2008
 * 
 * @brief  Matlab configuration (simple precision)
 * 
 * 
 */

#ifndef _RTA_CONFIGURATION_H_
#define _RTA_CONFIGURATION_H_ 1

/* Matlab Mex */
#include "mex.h"

/* Do not use Matlab memory management */
/* #undef rta_malloc */
/* #define rta_malloc mxMalloc */

/* #undef rta_realloc */
/* #define rta_realloc mxRealloc */

/* #undef rta_free */
/* #define rta_free mxFree */

/* simple floating point precision */
/* RTA and Matlab precisions must be the same */
/** RTA */
#undef RTA_REAL_TYPE
#define RTA_REAL_TYPE RTA_FLOAT_TYPE
/* #define RTA_REAL_TYPE RTA_DOUBLE_TYPE */

/** Matlab */
#undef RTA_MEX_REAL_TYPE
#define RTA_MEX_REAL_TYPE mxSINGLE_CLASS
/* #define RTA_MEX_REAL_TYPE mxDOUBLE_CLASS */

/* shared pointers */
#undef RTA_MEX_PTR_TYPE
#define RTA_MEX_PTR_TYPE mxUINT32_CLASS

#undef rta_ptr_t
#define rta_ptr_t unsigned long

#endif /* _RTA_CONFIGURATION_H_ */
