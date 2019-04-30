
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
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>

#include <debug/log.h>
#include <debug/memory.h>

#include <abz/pidfile.h>

int main ()
{
   char filename[128],answer[16],buf[32];
   pid_t pid = getpid ();
   int fd;
   ssize_t result;

   mem_open (NULL);
   log_open (NULL,LOG_NOISY,LOG_HAVE_COLORS | LOG_PRINT_FUNCTION);
   atexit (mem_close);
   atexit (log_close);

   sprintf (filename,"/tmp/test-%u.pid",pid);
   sprintf (answer,"%u\n",pid);

   if (pidfile (filename))
	 {
		log_printf (LOG_ERROR,"pidfile: %m\n");
		exit (EXIT_FAILURE);
	 }

   if ((fd = open (filename,O_RDONLY)) < 0)
	 {
		log_printf (LOG_ERROR,"open: %m\n");
		unlink (filename);
		exit (EXIT_FAILURE);
	 }

   if ((result = read (fd,buf,sizeof (buf))) != strlen (answer))
	 {
		if (result < 0)
		  log_printf (LOG_ERROR,"read: %m\n");
		else
		  log_printf (LOG_ERROR,
					  "short read count: expected %u bytes, got %u instead\n",
					  strlen (answer),result);

		unlink (filename);
		close (fd);
		exit (EXIT_FAILURE);
	 }

   close (fd);

   buf[result] = '\0';

   if (strcmp (buf,answer))
	 {
		log_printf (LOG_ERROR,"invalid pidfile contents\n");
		log_printf (LOG_DEBUG,"act='%s'\n",buf);
		log_printf (LOG_DEBUG,"exp='%s'\n",answer);
		unlink (filename);
		exit (EXIT_FAILURE);
	 }

   unlink (filename);

   log_printf (LOG_NORMAL,"pidfile() seems to work ok on this platform\n");

   exit (EXIT_SUCCESS);
}

