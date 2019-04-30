#ifndef ABZ_ARGV_H
#define ABZ_ARGV_H

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
 * Break up a string into NULL terminated list of arguments which
 * can be used for the exec() family of functions. Same rules as
 * that of most UNIX shells apply, i.e.
 *
 *       a) '\\' is the escape character
 *       b) '"' can be used to quote strings
 *       c) leading and trailing whitespace is stripped from each argument
 *
 * Returns a pointer to the NULL terminated argument list if
 * successful, or NULL if an error occurred. Call abz_get_error()
 * to retrieve error messages.
 */
extern char **strtoargv (const char *str);

/*
 * Same as strtoargv() with vprintf-style parameters. Returns a pointer
 * to the NULL terminated argument list if successful, or NULL if an
 * error occurred. Call abz_get_error() to retrieve error messages.
 */
extern char **vtoargv (const char *fmt,va_list ap);

/*
 * Same as strtoargv() with printf-style parameters. Returns a pointer
 * to the NULL terminated argument list if successful, or NULL if an
 * error occurred. Call abz_get_error() to retrieve error messages.
 */
extern char **toargv (const char *fmt, ...)
  __attribute__ ((format (printf,1,2)));

/*
 * Free memory allocated by strtoargv(), vtoargv() or toargv().
 */
extern void freeargv (char **argv);

/*
 * Concatenate all the arguments in the specified argument list
 * into a single string of specified size (arguments seperated
 * with spaces). The argv list must be NULL terminated.
 *
 * Due to the size constraint on `str', the possibility of ending
 * up with a truncated string obviously exists.
 */
extern void argvtostr (char *str,size_t size,char *const argv[]);

#endif	/* #ifndef ABZ_ARGV_H */
