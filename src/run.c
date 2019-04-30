
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

#include <stddef.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/wait.h>

#include <abz/bprintf.h>
#include <abz/sanitize.h>
#include <abz/tokens.h>
#include <abz/error.h>
#include <abz/argv.h>
#include <abz/run.h>

#include <debug/memory.h>

static __attribute__ ((noreturn)) void do_child (char *const argv[])
{
   int fd,result;

   /* close open file descriptors */

   if ((result = sysconf (_SC_OPEN_MAX)) <= 0)
	 _exit (127);

   while (result > 0)
	 close (--result);

   /* redirect output to /dev/null */

   if ((fd = open ("/dev/null",O_RDWR)) < 0)
	 _exit (127);

   dup2 (fd,STDIN_FILENO);
   dup2 (fd,STDOUT_FILENO);
   dup2 (fd,STDERR_FILENO);

   /* execute command */
   execvp (*argv,argv);

   /* shouldn't get here */
   close (fd);
   _exit (127);
}

static int do_parent (pid_t pid,char *const argv[])
{
   char buf[256];
   int result;

   for (;;)
	 {
		if (waitpid (pid,&result,0) < 0 && errno != EINTR)
		  {
			 abz_set_error ("waitpid %d failed: %m",pid);
			 return (-1);
		  }
		else if (WIFEXITED (result))
		  {
			 if (WEXITSTATUS (result))
			   {
				  abz_set_error ("child (%s) exited with non-zero exit status: %d",
								 *argv,WEXITSTATUS (result));

				  argvtostr (buf,sizeof (buf),argv);
				  abz_cat_error ("attempted command: %s",buf);

				  return (-1);
			   }

			 return (0);
		  }
	 }
}

int runv (char *const argv[])
{
   pid_t pid;

   if ((pid = fork ()) < 0)
	 {
		abz_set_error ("unable to fork process: %m");
		return (-1);
	 }

   if (pid > 0)
	 return (do_parent (pid,argv));

   do_child (argv);
}

int vrun (const char *fmt,va_list ap)
{
   int result;
   char **argv;

   if ((argv = vtoargv (fmt,ap)) == NULL)
	 return (-1);
   
   result = runv (argv);
   freeargv (argv);
   return (result);
}

int run (const char *fmt, ...)
{
   va_list ap;
   int result;

   va_start (ap,fmt);
   result = vrun (fmt,ap);
   va_end (ap);

   return (result);
}

