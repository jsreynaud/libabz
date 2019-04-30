
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
#include <stdlib.h>

#include <debug/log.h>
#include <debug/memory.h>

#include <abz/error.h>
#include <abz/tc.h>

static void fail (const char *fmt, ...)
{
   va_list ap;
   va_start (ap,fmt);
   if (fmt != NULL) log_vprintf (LOG_ERROR,fmt,ap);
   va_end (ap);
   exit (EXIT_FAILURE);
}

static int test (int print)
{
   struct batch tc;

   if (tc_create (&tc,print ? BATCH_TESTING : 0))
	 return (-1);

   if (tc_add (&tc,"qdisc del dev lo root") ||
	   tc_add (&tc,"qdisc del dev eth0 root") ||
	   tc_commit (&tc))
	 {
		tc_destroy (&tc);
		return (-1);
	 }

   return (0);
}

int main (void)
{
   mem_open (NULL);
   log_open (NULL,LOG_NOISY,LOG_HAVE_COLORS | LOG_PRINT_FUNCTION);
   atexit (mem_close);
   atexit (log_close);

   log_printf (LOG_DEBUG,"# print\n");

   if (test (1))
	 fail ("print: %s\n",abz_get_error ());

   log_printf (LOG_DEBUG,"# exec\n");

   if (test (0))
	 fail ("exec: %s\n",abz_get_error ());

   exit (EXIT_SUCCESS);
}

