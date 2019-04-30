
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

#include <debug/log.h>
#include <debug/memory.h>

#include <abz/typedefs.h>
#include <abz/error.h>
#include <abz/argv.h>

static void fail (const char *fmt, ...)
{
   va_list ap;
   va_start (ap,fmt);
   if (fmt != NULL) log_vprintf (LOG_ERROR,fmt,ap);
   va_end (ap);
   exit (EXIT_FAILURE);
}

static void test_argvtostr (void)
{
   static char *const argv1[] =
	 {
		"/bin/echo",
		"arg1",
		"arg2",
		"arg3",
		"\"foo bar\"",
		NULL
	 };
   static char *const argv2[] = { NULL };
   static const char res1[] = "/bin/echo arg1 arg2 arg3 \"foo bar\"";
   static const char res2[] = "/bin/echo arg1 arg2 arg3";
   static const char res3[] = "";
   char buf1[1024],buf2[strlen (res1) - 1],buf3[8];

   memset (buf1,255,sizeof (buf1));
   memset (buf2,255,sizeof (buf2));
   memset (buf3,255,sizeof (buf3));

   argvtostr (buf1,sizeof (buf1),argv1);

   if (strcmp (buf1,res1))
	 fail ("argvtostr #1 failed: result=\"%s\"\n",buf1);

   argvtostr (buf2,sizeof (buf2),argv1);

   if (strcmp (buf2,res2))
	 fail ("argvtostr #2 failed: result=\"%s\"\n",buf2);

   argvtostr (buf3,sizeof (buf3),argv2);

   if (strcmp (buf3,res3))
	 fail ("argvtostr #3 failed: result=\"%s\"\n",buf3);
}

static void test_strtoargv (void)
{
   static const char str[] = "   /bin/echo   arg1  arg2\\ arg3  \"foo bar\"   ";
   char **argv;
   static char *const res[] =
	 {
		"/bin/echo",
		"arg1",
		"arg2\\",
		"arg3",
		"foo bar",
		NULL
	 };
   size_t i;

   if ((argv = strtoargv (str)) == NULL)
	 fail ("strtoargv: %s\n",abz_get_error ());

   for (i = 0; argv[i] != NULL; i++) ;

   if (i != ARRAYSIZE (res) - 1)
	 {
		freeargv (argv);
		fail ("invalid number of arguments: %u/%u\n",i,ARRAYSIZE (res) - 1);
	 }

   for (i = 0; argv[i] != NULL; i++)
	 if (strcmp (argv[i],res[i]))
	   {
		  abz_set_error ("argv[%u] mismatch: exp=\"%s\", got=\"%s\"",i,argv[i],res[i]);
		  freeargv (argv);
		  fail ("%s\n",abz_get_error ());
	   }
}

int main ()
{
   mem_open (NULL);
   log_open (NULL,LOG_NOISY,LOG_HAVE_COLORS | LOG_PRINT_FUNCTION);
   atexit (mem_close);
   atexit (log_close);

   test_argvtostr ();
   test_strtoargv ();

   exit (EXIT_SUCCESS);
}

