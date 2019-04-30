
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

#include <debug/memory.h>

#include <abz/bcat.h>

/*
 * Concatenate all the arguments (followed by NULL) into one buffer.
 * Return a pointer to that buffer if successful, NULL if we ran out
 * of memory.
 */
char *bcat_stub (const char *filename,int line,const char *function,const char *arg, ...)
{
   va_list ap;
   char *a,*b,*c;
   size_t i,len;

   if (arg == NULL)
	 {
		errno = EINVAL;
		return (NULL);
	 }

   len = strlen (arg) + 1;
   if ((a = (char *) mem_alloc_stub (len * sizeof (char),filename,line,function)) == NULL)
	 return (NULL);
   strcpy (a,arg);

   va_start (ap,arg);

   for (i = 0; (b = va_arg (ap,char *)) != NULL; i++)
	 {
		len += strlen (b);
		if ((c = (char *) mem_realloc_stub (a,len * sizeof (char),filename,line,function)) == NULL)
		  {
			 int saved = errno;
			 mem_free (a);
			 errno = saved;
			 return (NULL);
		  }
		a = c;
		strcat (a,b);
	 }

   va_end (ap);

   return (a);
}

