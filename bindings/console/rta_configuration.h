/**
 * @file   rta_configuration_console.h
 * @author Diemo Schwarz
 * @date   8.12.2009
 *
 * @brief  console configuration for the rta library
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

#ifndef _RTA_CONFIGURATION_H_
#define _RTA_CONFIGURATION_H_ 1

#include "rta_stdio.h"
#include "rta_stdlib.h"

/** redefine console printing */
#undef rta_post
#define rta_post(args...) fprintf(stderr, ##args)
#include <stdio.h>

/** simple floating point precision */
#undef RTA_REAL_TYPE
#define RTA_REAL_TYPE RTA_FLOAT_TYPE

/* Apple VecLib for float and double */
#if defined(__APPLE__) && defined(__MACH__) && \
  (RTA_REAL_TYPE == RTA_FLOAT_TYPE || RTA_REAL_TYPE == RTA_DOUBLE_TYPE)
#define RTA_USE_VECLIB 1
#endif

#endif /* _RTA_CONFIGURATION_H_ */
