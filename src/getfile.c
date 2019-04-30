
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <debug/memory.h>

#include <abz/getfile.h>

#define SIZE 8192

void *getfile_stub (const char *file,int line,const char *function,const char *filename,size_t *count)
{
   int fd;
   void *buf,*ptr;
   size_t size = SIZE;
   ssize_t result;

   if ((fd = open (filename,O_RDONLY)) < 0)
	 return (NULL);

   *count = 0;

   if ((buf = mem_alloc_stub (size,file,line,function)) == NULL)
	 {
		int saved = errno;
		close (fd);
		errno = saved;
		return (NULL);
	 }

   while ((result = read (fd,buf + *count,size - *count)) > 0)
	 {
		*count += result;

		if (*count == size)
		  {
			 size += SIZE;
			 if ((ptr = mem_realloc_stub (buf,size,file,line,function)) == NULL)
			   {
				  int saved = errno;
				  close (fd);
				  mem_free (buf);
				  errno = saved;
				  return (NULL);
			   }
			 buf = ptr;
		  }
	 }

   if (result < 0)
	 {
		int saved = errno;
		close (fd);
		mem_free (buf);
		errno = saved;
		return (NULL);
	 }

   close (fd);

   return (buf);
}

