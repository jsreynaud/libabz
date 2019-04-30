#ifndef ABZ_BATCH_H
#define ABZ_BATCH_H

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

#include <stdio.h>

#define BATCH_TESTING	0x01
#define BATCH_FORCE		0x02
#define BATCH_FIRST		0x04

struct batch
{
   FILE *fp;
   int flags;
   char **argv;
};

/*
 * Create a batch of commands.
 *
 * The specified arguments specify the command that should be executed.
 * When executed, its output will be discarded and its stdin file descriptor
 * will be a file containing the list of commands added with batch_add() and
 * batch_append().
 *
 * Returns 0 if successful, -1 if some error occurred. Call abz_get_error()
 * to retrieve error messages.
 */
extern int batch_create (struct batch *batch,int flags,const char *fmt, ...)
  __attribute__ ((format (printf,3,4)));

/*
 * Add a command to the batch.
 *
 * Returns 0 if successful, -1 if some error occurred. Call abz_get_error()
 * to retrieve error messages.
 */
extern int batch_add (struct batch *batch,const char *fmt, ...)
  __attribute__ ((format (printf,2,3)));

/*
 * Append parameters to the last command in the batch.
 *
 * Returns 0 if successful, -1 if some error occurred. Call abz_get_error()
 * to retrieve error messages.
 */
extern int batch_append (struct batch *batch,const char *fmt, ...)
  __attribute__ ((format (printf,2,3)));

/*
 * Free resources allocated for this batch.
 */
extern void batch_destroy (struct batch *batch);

/*
 * Execute a batch of commands.
 *
 * This function automatically free resources allocated for the batch, i.e.
 * you should not call batch_destroy() after calling this function, except if
 * it fails.
 *
 * Returns 0 if successful, -1 if some error occurred. Call abz_get_error()
 * to retrieve error messages.
 */
extern int batch_commit (struct batch *batch);

#endif	/* #ifndef ABZ_BATCH_H */
