
/*
 * Copyright (c) Abraham vd Merwe <abz@blio.net>
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include <debug/memory.h>

#include <abz/getline.h>
#include <abz/alive.h>
#include <abz/atou32.h>

/*
 * Check if any process with name ``name'' is running. Returns 1 if so,
 * 0 if not, and -1 if some error occurred. Check errno to see what
 * error occurred.
 */
int alive (const char *name)
{
   DIR *dir = NULL;
   struct dirent *entry;
   int fd,__errno_saved;
   char filename[PATH_MAX],*process;
   pid_t me = getpid ();
   uint32_t pid;

   if ((dir = opendir ("/proc")) == NULL)
	 return (-1);

   while ((entry = readdir (dir)) != NULL)
	 if (!atou32 (entry->d_name,&pid) && pid != me)
	   {
		  if (strlen (entry->d_name) + 14 >= PATH_MAX)
			{
			   closedir (dir);
			   errno = ENAMETOOLONG;
			   return (-1);
			}

		  strcpy (filename,"/proc/");
		  strcat (filename,entry->d_name);
		  strcat (filename,"/status");

		  if ((fd = open (filename,O_RDONLY)) < 0)
			break;

		  if ((process = getline (fd)) == NULL && errno)
			break;

		  if (process != NULL && !strncmp ("Name:\t",process,6))
			{
			   process[strlen (process)] = '\0';

			   if (!strcmp (name,process + 6))
				 {
					mem_free (process);

					/* we have to be paranoid here, otherwise we might leak file descriptors */
					if (close (fd) < 0 || closedir (dir) < 0)
					  break;

					return (1);
				 }

			   if (process != NULL)
				 mem_free (process);

			   if (close (fd) < 0)
				 break;
			}
	   }

   if (entry != NULL)
	 {
		__errno_saved = errno;

		if (dir != NULL)
		  closedir (dir);

		if (fd > 0)
		  close (fd);

		errno = __errno_saved;
		return (-1);
	 }

   /* once again, paranoia is good (: */
   if (closedir (dir) < 0)
	 return (-1);

   return (0);
}

