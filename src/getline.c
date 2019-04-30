
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

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <debug/memory.h>

#include <abz/getline.h>

/* initial line length */
#define LINELEN		128

/* return the next line in the file, or NULL if EOF or an error occurred */
char *getline_stub (const char *filename,int line,const char *function,int fd)
{
   char *ptr,*buf;
   int len = LINELEN;
   char c;
   int n = 0;
   ssize_t result;

   if ((buf = (char *) mem_alloc_stub (len * sizeof (char),filename,line,function)) == NULL)
	 return (NULL);

   do
	 {
		result = read (fd,&c,1);
		if (result < 0)
		  {
			 int __errno_saved = errno;
			 mem_free (buf);
			 errno = __errno_saved;
			 return (NULL);
		  }
		else if (result > 0 && c != '\n')
		  {
			 if (n >= len - 1)
			   {
				  len += LINELEN;
				  if ((ptr = (char *) mem_realloc_stub (buf,len * sizeof (char),filename,line,function)) == NULL)
					{
					   int __errno_saved = errno;
					   mem_free (buf);
					   errno = __errno_saved;
					   return (NULL);
					}
				  buf = ptr;
			   }
			 buf[n++] = c;
		  }
	 }
   while (result && c != '\n');
   buf[n] = '\0';

   len = strlen (buf);

   if (!result && !len)
	 {
		mem_free (buf);
		errno = 0;
		return (NULL);
	 }

   /* if possible try not to waste memory */
   if ((ptr = (char *) mem_realloc_stub (buf,(len + 1) * sizeof (char),filename,line,function)) == NULL)
	 return (buf);

   return (ptr);
}

