
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
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <debug/log.h>
#include <debug/memory.h>

#include <abz/tokens.h>
#include <abz/sanitize.h>
#include <abz/getline.h>
#include <abz/error.h>

static void parse (int fd)
{
   char *line;
   int i,n = 0;
   struct tokens tokens;

   while ((line = getline (fd)) != NULL)
	 {
		n++;

		sanitize (line);
		if (*line == '\0')
		  {
			 mem_free (line);
			 continue;
		  }

		if (tokens_parse (&tokens,line))
		  {
			 log_printf (LOG_ERROR,"parse error on line %d: %s\n",n,abz_get_error ());
			 mem_free (line);
			 return;
		  }

		for (i = 0; i < tokens.argc; i++)
		  log_printf (LOG_NORMAL,"line[%d].argv[%d]=\"%s\"\n",n - 1,i,tokens.argv[i]);

		tokens_destroy (&tokens);
		mem_free (line);
	 }
}

int main (int argc,char *argv[])
{
   int fd;

   log_open (NULL,LOG_NORMAL,LOG_HAVE_COLORS);
   atexit (mem_close);
   atexit (log_close);

   if (argc != 2)
	 {
		const char *progname;
		(progname = strrchr (argv[0],'/')) != NULL ? progname++ : (progname = argv[0]);
		log_printf (LOG_ERROR,"usage: %s <filename>\n",progname);
		exit (EXIT_FAILURE);
	 }

   if ((fd = open (argv[1],O_RDONLY)) < 0)
	 {
		log_printf (LOG_ERROR,"couldn't open file %s for reading!\n",argv[1]);
		exit (EXIT_FAILURE);
	 }

   parse (fd);

   close (fd);

   exit (EXIT_SUCCESS);
}

