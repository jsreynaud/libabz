
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
#include <string.h>

#include <debug/memory.h>

#include <abz/error.h>
#include <abz/bcat.h>
#include <abz/bprintf.h>
#include <abz/argv.h>

static void out_of_memory (void)
{
   abz_set_error ("failed to allocate memory: %m");
}

void freeargv (char **argv)
{
   if (argv != NULL)
	 {
		int i;

		for (i = 0; argv[i] != NULL; i++)
		  mem_free (argv[i]);

		mem_free (argv);
	 }
}

static __inline__ void moveback (char *string)
{
   while (*string != '\0')
	 string[0] = string[1], string++;
}

static char *getnext (char **string)
{
   char *head = *string,*tail;
   int quote = 0;

   /* strip leading spaces */
   while (*head == ' ' || *head == '\t')
	 head++;

   tail = head;

   for (;;)
	 {
		switch (*tail)
		  {
			 /* escape character */
		   case '\\':
			 if (tail[1] == '\\' || tail[1] == '\'' || tail[1] == '"') moveback (tail);
			 tail++;
			 break;

			 /* single quotes */
		   case '\'':
			 moveback (tail);
			 while (*tail != '\'' && *tail != '\0') tail++;
			 if (*tail == '\'') moveback (tail);
			 break;

			 /* double quote */
		   case '"':
			 quote = !quote;
			 moveback (tail);
			 break;

			 /* end-of-string */
		   case '\0':
			 *string = tail;
			 return (head != tail ? head : NULL);

			 /* white space */
		   case ' ':
		   case '\t':
			 if (!quote)
			   {
				  *tail = '\0';
				  *string = tail + 1;
				  return (head != tail ? head : NULL);
			   }

			 /* any other character */
		   default:
			 tail++;
		  }
	 }
}

static char **__strtoargv (char *str)
{
   char *token,**argv = NULL;
   int nargs = 0,argc = 0;

   while ((token = getnext (&str)) != NULL)
	 {
		if (argc >= nargs - 1)
		  {
			 void *ptr;

			 nargs = (nargs + 1) << 1;

			 if ((ptr = mem_realloc (argv,nargs * sizeof (*argv))) == NULL)
			   {
				  out_of_memory ();
				  freeargv (argv);
				  return (NULL);
			   }

			 argv = ptr;
		  }

		if ((argv[argc++] = bcat (token,NULL)) == NULL)
		  {
			 out_of_memory ();
			 freeargv (argv);
			 return (NULL);
		  }

		argv[argc] = NULL;
	 }

   return (argv);
}

char **strtoargv (const char *str)
{
   char *s,**argv;

   if ((s = bcat (str,NULL)) == NULL)
	 {
		out_of_memory ();
		return (NULL);
	 }

   argv = __strtoargv (s);
   mem_free (s);
   return (argv);
}

void argvtostr (char *str,size_t size,char *const argv[])
{
   size_t i,len = 1;
   char *s = str;

   if (str == NULL || !size || argv == NULL)
	 return;

   *s = '\0';

   for (i = 0; argv[i] != NULL; i++)
	 {
		len = strlen (argv[i]);

		if (size > len + (i ? 1 : 0))
		  {
			 if (i)
			   *s++ = ' ', size--;

			 strcpy (s,argv[i]);
			 s += len, size -= len;
		  }
	 }
}

char **vtoargv (const char *fmt,va_list ap)
{
   char *str,**argv;

   if ((str = vbprintf (fmt,ap)) == NULL)
	 {
		out_of_memory ();
		return (NULL);
	 }

   argv = __strtoargv (str);
   mem_free (str);
   return (argv);
}

char **toargv (const char *fmt, ...)
{
   va_list ap;
   char **argv;

   va_start (ap,fmt);
   argv = vtoargv (fmt,ap);
   va_end (ap);

   return (argv);
}

