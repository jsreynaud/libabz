#ifndef ABZ_ERROR_H
#define ABZ_ERROR_H

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

/*
 * Set error message. Once set, the message has to be cleared before
 * another abz_set_error() will change the error message.
 */
extern void abz_set_error (const char *fmt, ...)
  __attribute__ ((format (printf,1,2)));

/*
 * Append and error message. If an error already exist, a newline will be
 * inserted between the two error messages. The usual rule of set-once-clear
 * does not apply here (i.e. if you call this consecutive times, the error
 * message will span multiple lines).
 */
extern void abz_cat_error (const char *fmt, ...)
  __attribute__ ((format (printf,1,2)));

/*
 * Return the first error message that was set since the last
 * abz_clear_error() call. If no error exists (i.e.
 * abz_set_error() was not called yet), it returns an
 * empty string ("").
 */
extern const char *abz_get_error ();

/*
 * Clear error.
 */
extern void abz_clear_error ();

#endif	/* #ifndef ABZ_ERROR_H */
