
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <debug/log.h>
#include <debug/memory.h>

#include <abz/getline.h>

static void fail (const char *fmt, ...)
{
   va_list ap;
   va_start (ap,fmt);
   if (fmt != NULL) log_vprintf (LOG_ERROR,fmt,ap);
   va_end (ap);
   exit (EXIT_FAILURE);
}

int main (int argc,char *argv[])
{
   int fd,line = 1;
   char *buf;

   mem_open (NULL);
   log_open (NULL,LOG_NOISY,LOG_HAVE_COLORS | LOG_PRINT_FUNCTION);
   atexit (mem_close);
   atexit (log_close);

   if (argc != 2)
	 {
		const char *progname;
		(progname = strrchr (argv[0],'/')) != NULL ? progname++ : argv[0];
		fail ("usage: %s <filename>\n",progname);
	 }

   if ((fd = open (argv[1],O_RDONLY)) < 0)
	 fail ("open(): %s\n",strerror (errno));

   while ((buf = getline (fd)) != NULL)
	 {
		log_printf (LOG_DEBUG,"line %.4d \"%s\"\n",line++,buf);
		mem_free (buf);
	 }

   if (errno)
	 {
		log_printf (LOG_ERROR,"getline(): %s\n",strerror (errno));
		close (fd);
		fail (NULL);
	 }

   close (fd);

   exit (EXIT_SUCCESS);
}

