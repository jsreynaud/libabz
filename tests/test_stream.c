
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

#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <debug/log.h>
#include <debug/memory.h>

#include <abz/error.h>
#include <abz/stream.h>
#include <abz/run.h>

static void fail (const char *fmt, ...)
{
   va_list ap;
   va_start (ap,fmt);
   if (fmt != NULL) log_vprintf (LOG_ERROR,fmt,ap);
   va_end (ap);
   exit (EXIT_FAILURE);
}

int main (void)
{
   struct stream istream,ostream;
   ssize_t result,res2;
   const char infile[] = "test_parse.txt";
   const char outfile[] = "test_stream.txt";
   size_t n = 0;
   char *line;
   int infd,outfd;

   mem_open (NULL);
   log_open (NULL,LOG_NOISY,LOG_HAVE_COLORS | LOG_PRINT_FUNCTION);
   atexit (mem_close);
   atexit (log_close);

   if ((infd = open (infile,O_RDONLY)) < 0)
	 fail ("failed to open %s for reading\n",infile);

   if ((outfd = creat (outfile,0644)) < 0)
	 {
		close (infd);
		fail ("failed to create %s\n",outfile);
	 }

   stream_open (&istream,infd);
   stream_open (&ostream,outfd);

   while ((result = stream_getline (&istream,&line)) > 0)
	 {
		if ((res2 = stream_write (&ostream,line,strlen (line))) < 0 || res2 != strlen (line))
		  {
			 stream_close (&istream);
			 stream_close (&ostream);
			 unlink (outfile);

			 if (res2 < 0)
			   fail ("write error: %s\n",abz_get_error ());
			 else
			   fail ("short write count: %u/%u bytes written\n",res2,strlen (line));
		  }

		line[strlen (line) - 1] = '\0';
		log_printf (LOG_NORMAL,"line %u: \"%s\"\n",++n,line);
		mem_free (line);
	 }

   stream_close (&istream);
   stream_close (&ostream);

   if (result < 0)
	 {
		unlink (outfile);
		fail ("read error: %s\n",abz_get_error ());
	 }

   if (run ("/usr/bin/cmp -s %s %s",infile,outfile))
	 {
		unlink (outfile);
		fail ("%s\n",abz_get_error ());
	 }

   unlink (outfile);

   exit (EXIT_SUCCESS);
}

