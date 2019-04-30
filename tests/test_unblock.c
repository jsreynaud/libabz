
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

#include <signal.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <debug/log.h>
#include <debug/memory.h>

#include <abz/error.h>
#include <abz/unblock.h>

static void fail (const char *fmt, ...)
{
   va_list ap;
   va_start (ap,fmt);
   if (fmt != NULL) log_vprintf (LOG_ERROR,fmt,ap);
   va_end (ap);
   exit (EXIT_FAILURE);
}

static const char filename[] = "test_unblock.fifo";
static int fd;

static void ignore (int sig)
{
   log_printf (LOG_NORMAL,"back again. seems to be working\n");
   close (fd);
   unlink (filename);
   exit (EXIT_SUCCESS);
}

int main (void)
{
   char buf[1024];
   ssize_t result;

   mem_open (NULL);
   log_open (NULL,LOG_NOISY,LOG_HAVE_COLORS | LOG_PRINT_FUNCTION);
   atexit (mem_close);
   atexit (log_close);

   unlink (filename);

   if ((fd = mkfifo (filename,0644)) < 0)
	 fail ("failed to create %s: %m\n",filename);

   log_printf (LOG_NORMAL,"entering non-blocking mode\n");

   if (unblock (fd))
	 {
		close (fd);
		fail ("%s\n",abz_get_error ());
	 }

   result = read (fd,buf,sizeof (buf));
   log_printf (LOG_NORMAL,"back again. result=%d, errno=%d (EAGAIN=%d)\n",result,errno,EAGAIN);

   log_printf (LOG_NORMAL,"entering blocking mode\n");

   if (block (fd))
	 {
		close (fd);
		fail ("%s\n",abz_get_error ());
	 }

   signal (SIGALRM,ignore);
   alarm (5);

   result = read (fd,buf,sizeof (buf));
   log_printf (LOG_ERROR,"read returned: result=%d, errno=%u (%m)\n",result,errno);

   close (fd);
   unlink (filename);
   exit (EXIT_FAILURE);
}

