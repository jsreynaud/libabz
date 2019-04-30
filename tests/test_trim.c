
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

#include <abz/trim.h>

static void fail (const char *fmt, ...)
{
   va_list ap;
   va_start (ap,fmt);
   if (fmt != NULL) log_vprintf (LOG_ERROR,fmt,ap);
   va_end (ap);
   exit (EXIT_FAILURE);
}

static void test_trim (const char *answer,const char *str)
{
   char buf[1024];
   strcpy (buf,str);
   trim (buf);
   if (strcmp (answer,buf))
	 fail ("trim: expected ``%s'', got ``%s'' instead!\n",answer,buf);
}

static void test_ltrim (const char *answer,const char *str)
{
   char buf[1024];
   strcpy (buf,str);
   ltrim (buf);
   if (strcmp (answer,buf))
	 fail ("ltrim: expected ``%s'', got ``%s'' instead!\n",answer,buf);
}

static void test_rtrim (const char *answer,const char *str)
{
   char buf[1024];
   strcpy (buf,str);
   rtrim (buf);
   if (strcmp (answer,buf))
	 fail ("rtrim: expected ``%s'', got ``%s'' instead!\n",answer,buf);
}

int main ()
{
   mem_open (NULL);
   log_open (NULL,LOG_NOISY,LOG_HAVE_COLORS | LOG_PRINT_FUNCTION);
   atexit (mem_close);
   atexit (log_close);

   test_trim ("stripped","stripped");
   test_trim ("blah","  blah");
   test_trim ("blah","\tblah");
   test_trim ("blah","\nblah");
   test_trim ("blah"," \t \nblah");
   test_trim ("blah","  blah ");
   test_trim ("blah","  blah \n");
   test_trim ("blah","  blah \t");
   test_trim ("blah","  blah\t");
   test_trim ("blah","  blah\n\n\n");

   log_printf (LOG_NORMAL,"trim() seems to work ok on this platform\n");

   test_ltrim ("stripped","stripped");
   test_ltrim ("blah","  blah");
   test_ltrim ("blah","\tblah");
   test_ltrim ("blah","\nblah");
   test_ltrim ("blah"," \t \nblah");
   test_ltrim ("blah ","  blah ");
   test_ltrim ("blah \n","  blah \n");
   test_ltrim ("blah \t","  blah \t");
   test_ltrim ("blah\t","  blah\t");
   test_ltrim ("blah\n\n\n","  blah\n\n\n");

   log_printf (LOG_NORMAL,"ltrim() seems to work ok on this platform\n");

   test_rtrim ("stripped","stripped");
   test_rtrim ("  blah","  blah");
   test_rtrim ("\tblah","\tblah");
   test_rtrim ("\nblah","\nblah");
   test_rtrim ("\t \nblah","\t \nblah");
   test_rtrim ("  blah","  blah ");
   test_rtrim ("  blah","  blah \n");
   test_rtrim ("  blah","  blah \t");
   test_rtrim ("  blah","  blah\t");
   test_rtrim ("  blah","  blah\n\n\n");

   log_printf (LOG_NORMAL,"rtrim() seems to work ok on this platform\n");

   exit (EXIT_SUCCESS);
}

