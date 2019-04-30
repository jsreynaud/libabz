
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
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <debug/memory.h>

#include <abz/mkdirhier.h>

/*
 * create a directory hierarchy. return -1 if some error occurred, or 0 if
 * successful. check errno to see what error occurred.
 */
int mkdirhier (const char *dir)
{
   const char *prev = dir,*next;
   char *buf;
   struct stat sb;
   mode_t mode = S_IRWXU | S_IXGRP | S_IRGRP | S_IXOTH | S_IROTH;

   if ((buf = (char *) mem_alloc ((strlen (dir) + 1) * sizeof (char))) == NULL)
	 return (-1);

   do
	 {
		next = prev;
		while (*next != '\0' && *next != '/') next++;

		if (*next != '\0')
		  {
			 strncpy (buf,dir,next - dir);
			 buf[next - dir] = '\0';

			 if (*buf != '\0' && stat (buf,&sb) < 0)
			   if (mkdir (buf,mode) < 0)
				 {
					int saved = errno;
					mem_free (buf);
					errno = saved;
					return (-1);
				 }

			 prev = next + 1;
		  }
	 }
   while (*next != '\0');

   mem_free (buf);

   return (mkdir (dir,mode));
}

