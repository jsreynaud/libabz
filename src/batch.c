
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

/* needed for environ */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <debug/log.h>
#include <debug/memory.h>

#include <abz/error.h>
#include <abz/tokens.h>
#include <abz/bprintf.h>
#include <abz/bcat.h>
#include <abz/sanitize.h>
#include <abz/batch.h>
#include <abz/iptables.h>
#include <abz/tc.h>

static void out_of_memory (void)
{
   abz_set_error ("failed to allocate memory: %m");
}

static void write_error (void)
{
   abz_set_error ("failed to write to temporary file: %m");
}

static __inline__ char **getargs (const char *fmt,va_list ap)
{
   char *str,**argv;
   struct tokens tokens;
   size_t i;

   if ((str = vbprintf (fmt,ap)) == NULL)
	 {
		out_of_memory ();
		return (NULL);
	 }

   sanitize (str);

   if (tokens_parse (&tokens,str) < 0)
	 {
		mem_free (str);
		return (NULL);
	 }

   mem_free (str);

   if ((argv = mem_alloc (sizeof (*argv) * (tokens.argc + 2))) == NULL)
	 {
		out_of_memory ();
		tokens_destroy (&tokens);
		return (NULL);
	 }

   for (i = 0; i < tokens.argc; i++)
	 argv[i + 1] = tokens.argv[i];

   argv[0] = argv[1];
   (str = strrchr (argv[1],'/')) != NULL ? str++ : (str = argv[1]);

   if ((argv[1] = bcat (str,NULL)) == NULL)
	 {
		out_of_memory ();
		tokens_destroy (&tokens);
		mem_free (argv);
		return (NULL);
	 }

   argv[tokens.argc + 1] = NULL;
   mem_free (tokens.argv);
   return (argv);
}

int batch_create (struct batch *batch,int flags,const char *fmt, ...)
{
   va_list ap;

   memset (batch,0L,sizeof (*batch));
   batch->flags = flags | BATCH_FIRST;

   va_start (ap,fmt);
   batch->argv = getargs (fmt,ap);
   va_end (ap);

   if (batch->argv == NULL)
	 {
		out_of_memory ();
		return (-1);
	 }

   if ((batch->fp = tmpfile ()) == NULL)
	 {
		abz_set_error ("failed to create temporary file: %m");
		return (-1);
	 }

   return (0);
}

static int newline (struct batch *batch)
{
   if (!(batch->flags & BATCH_FIRST))
	 {
		if (fputc ('\n',batch->fp) == EOF)
		  {
			 write_error ();
			 return (-1);
		  }
	 }
   else batch->flags &= ~BATCH_FIRST;

   return (0);
}

int batch_add (struct batch *batch,const char *fmt, ...)
{
   va_list ap;
   int result;

   if (newline (batch))
	 return (-1);

   va_start (ap,fmt);
   result = vfprintf (batch->fp,fmt,ap);
   va_end (ap);

   if (result == EOF)
	 {
		write_error ();
		return (-1);
	 }

   return (0);
}

int batch_append (struct batch *batch,const char *fmt, ...)
{
   va_list ap;
   int result;

   if (fputc (' ',batch->fp) == EOF)
	 {
		write_error ();
		return (-1);
	 }

   va_start (ap,fmt);
   result = vfprintf (batch->fp,fmt,ap);
   va_end (ap);

   if (result == EOF)
	 {
		write_error ();
		return (-1);
	 }

   return (0);
}

void batch_destroy (struct batch *batch)
{
   size_t i;

   fclose (batch->fp);

   for (i = 0; batch->argv[i] != NULL; i++)
	 mem_free (batch->argv[i]);

   mem_free (batch->argv);
}

static int print (int fd,char *const argv[],int force)
{
   char buf[1024];
   size_t i;
   ssize_t result;

   log_puts (LOG_NORMAL,*argv);

   for (i = 2; argv[i] != NULL; i++)
	 log_printf (LOG_NORMAL," %s",argv[i]);

   log_puts (LOG_NORMAL," << EOF\n");

   while ((result = read (fd,buf,sizeof (buf) - 1)) > 0)
	 {
		buf[result] = '\0';
		log_puts (LOG_NORMAL,buf);
	 }

   if (result < 0)
	 {
		abz_set_error ("failed to read from temporary file: %m");
		return (-1);
	 }

   log_puts (LOG_NORMAL,"EOF\n");
   return (0);
}

static __attribute__ ((noreturn)) void child (int infd,char *const argv[],int force)
{
   int fd;

   /* close all unused file descriptors */

   if ((fd = sysconf (_SC_OPEN_MAX)) <= 0)
	 _exit (127);

   while (fd > 0)
	 {
		--fd;

		if (fd != infd)
		  close (fd);
	 }

   /* redirect output to /dev/null */

   if ((fd = open ("/dev/null",O_WRONLY)) < 0)
	 _exit (127);

   /* execute command */

   dup2 (infd,STDIN_FILENO);
   dup2 (fd,STDOUT_FILENO);
   dup2 (fd,STDERR_FILENO);
   execve (*argv,argv + 1,environ);
   _exit (127);
}

static int run (int fd,char *const argv[],int force)
{
   pid_t pid;
   int result;

   if ((pid = fork ()) < 0)
	 {
		if (!force)
		  abz_set_error ("unable to fork process: %m");

		return (-1);
	 }

   if (!pid)
	 child (fd,argv,force);

   for (;;)
	 {
		if (waitpid (pid,&result,0) < 0 && errno != EINTR)
		  {
			 abz_set_error ("waitpid %d failed: %m",pid);
			 return (-1);
		  }
		else if (WIFEXITED (result))
		  {
			 if (WEXITSTATUS (result) && !force)
			   {
				  abz_set_error ("child (%s) exited with non-zero exit status: %d",*argv,WEXITSTATUS (result));
				  return (-1);
			   }

			 return (0);
		  }
	 }
}

int batch_commit (struct batch *batch)
{
   int (*callback) (int fd,char *const argv[],int force);

   if (newline (batch))
	 return (-1);

   if (fflush (batch->fp) == EOF)
	 {
		write_error ();
		return (-1);
	 }

   rewind (batch->fp);
   callback = (batch->flags & BATCH_TESTING) ? print : run;

   if (callback (fileno (batch->fp),batch->argv,batch->flags & BATCH_FORCE))
	 return (-1);

   batch_destroy (batch);
   return (0);
}

int iptables_add (struct batch *batch,const char *fmt, ...)
  __attribute__ ((weak, alias ("batch_add")));

int iptables_append (struct batch *batch,const char *fmt, ...)
  __attribute__ ((weak, alias ("batch_append")));

void iptables_destroy (struct batch *batch)
  __attribute__ ((weak, alias ("batch_destroy")));

int tc_add (struct batch *batch,const char *fmt, ...)
  __attribute__ ((weak, alias ("batch_destroy")));

int tc_append (struct batch *batch,const char *fmt, ...)
  __attribute__ ((weak, alias ("batch_append")));

void tc_destroy (struct batch *batch)
  __attribute__ ((weak, alias ("batch_destroy")));

int tc_commit (struct batch *batch)
  __attribute__ ((weak, alias ("batch_commit")));

