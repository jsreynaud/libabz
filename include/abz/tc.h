#ifndef ABZ_TC_H
#define ABZ_TC_H

/*
 * Copyright (c) Abraham vd Merwe <abz@blio.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of other contributors
 *	  may be used to endorse or promote products derived from this software
 *	  without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "batch.h"

/*
 * Create tc batch.
 *
 * Returns 0 if successful, -1 if some error occurred. Call abz_get_error()
 * to retrieve error messages.
 */
extern int tc_create (struct batch *batch,int flags);

/*
 * Add an tc command to the batch.
 *
 * Returns 0 if successful, -1 if some error occurred. Call abz_get_error()
 * to retrieve error messages.
 */
extern int tc_add (struct batch *batch,const char *fmt, ...)
  __attribute__ ((format (printf,2,3)));

/*
 * Append parameters to the last tc command in the batch.
 *
 * Returns 0 if successful, -1 if some error occurred. Call abz_get_error()
 * to retrieve error messages.
 */
extern int tc_append (struct batch *batch,const char *fmt, ...)
  __attribute__ ((format (printf,2,3)));

/*
 * Free resources allocated for this tc batch.
 */
extern void tc_destroy (struct batch *batch);

/*
 * Execute tc batch.
 *
 * This function automatically free resources allocated for the batch, i.e.
 * you should not call tc_destroy() after calling this function, even if
 * it fails.
 *
 * Returns 0 if successful, -1 if some error occurred. Call abz_get_error()
 * to retrieve error messages.
 */
extern int tc_commit (struct batch *batch);

#endif	/* #ifndef ABZ_TC_H */
