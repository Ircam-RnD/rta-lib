/**
 * @file   rta_types.h
 * @author Jean-Philippe Lambert
 * @date   Mon Sep 10 11:05:09 2007
 * @ingroup rta_util
 *
 * @brief  Type wrapper for float, double or long double
 *
 * Default is RTA_REAL_FLOAT. Define your RTA_REAL_TYPE to override these.
 *
 * @copyright
 * Copyright (C) 2007 by IRCAM-Centre Georges Pompidou, Paris, France.
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

#ifndef _RTA_TYPES_H_
#define _RTA_TYPES_H_ 1

/** default floating point precision */
#ifndef RTA_REAL_TYPE
#define RTA_REAL_TYPE RTA_FLOAT_TYPE
#endif

#undef RTA_FLOAT_TYPE
#define RTA_FLOAT_TYPE 1
#undef RTA_DOUBLE_TYPE
#define RTA_DOUBLE_TYPE 2
#undef RTA_LONG_DOUBLE_TYPE
#define RTA_LONG_DOUBLE_TYPE 3

#if (RTA_REAL_TYPE == RTA_FLOAT_TYPE)
#undef rta_real_t
#define rta_real_t float
#endif

#if (RTA_REAL_TYPE == RTA_DOUBLE_TYPE)
#undef rta_real_t
#define rta_real_t double
#endif

#if (RTA_REAL_TYPE == RTA_LONG_DOUBLE_TYPE)
#undef rta_real_t
#define rta_real_t long double
#endif

#endif /* _RTA_TYPES_H_ */
