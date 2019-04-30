
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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#include <debug/memory.h>

#include <abz/bprintf.h>

/* va_copy is __va_copy in older versions of GCC. */
#if defined __GNUC__ && __GNUC__ < 3
#define va_copy(d,s) __va_copy(d,s)
#endif

/*
 * Print fmt and it's arguments (vprintf-style) to a newly allocated
 * buffer and return a pointer to that buffer if successful, NULL
 * if we ran out of memory.
 */
char *vbprintf_stub (const char *filename,int line,const char *function,const char *fmt,va_list ap)
{
   /* guess we need no more than 100 bytes */
   int n,size = 100;
   char *buf,*ptr;
   va_list ap_copy;

   if (fmt == NULL)
	 {
		errno = EINVAL;
		return (NULL);
	 }

   if (*fmt == '\0')
	 {
		if ((buf = (char *) mem_alloc_stub (sizeof (char),filename,line,function)) == NULL)
		  return (NULL);
		*buf = '\0';
		return (buf);
	 }

   if ((buf = (char *) mem_alloc_stub (size * sizeof (char),filename,line,function)) == NULL)
	 return (NULL);

   for (;;)
	 {
		/* try to print in the allocated space */
		va_copy (ap_copy,ap);
		n = vsnprintf (buf,size,fmt,ap_copy);
		va_end (ap_copy);

		/* if that worked, we're finished */
		if (n > -1 && n < size) break;

		/* else try again with more space */
		if (n > -1)		/* glibc 2.1 */
		  size = n + 1;	/* precisely what is needed */
		else			/* glibc 2.0 */
		  size *= 2;	/* twice the old size */

		if ((ptr = (char *) mem_realloc_stub (buf,size * sizeof (char),filename,line,function)) == NULL)
		  {
			 int saved = errno;
			 mem_free (buf);
			 errno = saved;
			 return (NULL);
		  }
		else buf = ptr;
	 }

   return (buf);
}

/*
 * Print fmt and it's arguments (printf-style) to a newly allocated
 * buffer and return a pointer to that buffer if successful, NULL
 * if we ran out of memory.
 */
char *bprintf_stub (const char *filename,int line,const char *function,const char *fmt, ...)
{
   char *buf;
   va_list ap;
   va_start (ap,fmt);
   buf = vbprintf_stub (filename,line,function,fmt,ap);
   va_end (ap);
   return (buf);
}

