
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
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#include <debug/log.h>
#include <debug/memory.h>

#include <abz/tmp.h>

static void fail (const char *fmt, ...)
{
   va_list ap;
   va_start (ap,fmt);
   if (fmt != NULL) log_vprintf (LOG_ERROR,fmt,ap);
   va_end (ap);
   exit (EXIT_FAILURE);
}

static volatile int done = 0;
static void catcher (int sig)
{
   done = 1;
}

int main (int argc,char *argv[])
{
   int i,j = 0;
   char *s;
   const char *progname;

   mem_open (NULL);
   log_open (NULL,LOG_NOISY,LOG_HAVE_COLORS | LOG_PRINT_FUNCTION);
   atexit (mem_close);
   atexit (log_close);

   (progname = strrchr (argv[0],'/')) != NULL ? progname++ : argv[0];

   if (tmp_open ("/tmp",progname) < 0)
	 fail ("tmp_open(): %s\n",strerror (errno));
   atexit (tmp_close);

   signal (SIGINT,catcher);
   signal (SIGTERM,catcher);

   while (!done)
	 {
		for (i = 0; i < 3; i++)
		  {
			 if ((s = tmp_name ()) == NULL) fail ("tmp_name(): %s\n",strerror (errno));
			 log_printf (LOG_DEBUG,"%s  ",s);
			 mem_free (s);
		  }

		log_putc (LOG_DEBUG,'\n');

		if (j++ > 5)
		  {
			 sleep (1);
			 j = 0;
		  }
	 }

   exit (EXIT_SUCCESS);
}

