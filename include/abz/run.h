#ifndef ABZ_RUN_H
#define ABZ_RUN_H

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

#include <stdarg.h>

/*
 * Execute a command with the specified exec() style NULL terminated
 * argument list. All input and output of the specified program is
 * directed to /dev/null (discarded). Returns 0 if successful, -1 if
 * some error occurred. Call abz_get_error() to retrieve error messages.
 */
extern int runv (char *const argv[]);

/*
 * Execute a command with the specified vprintf-style parameters.
 * All input and output of the specified program is directed to
 * /dev/null (discarded). Returns 0 if successful, -1 if some
 * error occurred. Call abz_get_error() to retrieve error messages.
 */
extern int vrun (const char *fmt,va_list ap);

/*
 * Execute a command with the specified printf-style parameters.
 * All input and output of the specified program is directed to
 * /dev/null (discarded). Returns 0 if successful, -1 if some
 * error occurred. Call abz_get_error() to retrieve error messages.
 */
extern int run (const char *fmt, ...)
  __attribute__ ((format (printf,1,2)));

#endif	/* #ifndef ABZ_RUN_H */
