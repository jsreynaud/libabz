
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
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>

#include <debug/memory.h>

#include <abz/bcat.h>
#include <abz/tmp.h>

#define STRLEN(x) (sizeof (x) / sizeof ((x)[0]) - 1)
#define RETRIES (STRLEN (letters) << STRLEN (padding))

static const char padding[] = "XXXXXXX";
static const char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

static int make_temp (char *path,int create)
{
   uint64_t tmp;
   static uint64_t value = 0;
   int i,j,len,fd = -1;
   char *XXX;
   struct timeval tv;
   struct stat st;

   if ((len = strlen (path) - STRLEN (padding)) < 0)
	 {
		errno = EINVAL;
		return (-1);
	 }

   XXX = path + len;
   gettimeofday (&tv,NULL);
   value += ((uint64_t) tv.tv_usec << 16) ^ tv.tv_sec ^ getpid () ^ getppid ();

   for (i = 0; i < RETRIES; value += 7777777, i++)
	 {
		tmp = value;
		for (j = 0; j < STRLEN (padding); j++)
		  XXX[j] = letters[tmp % STRLEN (letters)], tmp /= STRLEN (letters);

		if (!create)
		  {
			 if (lstat (path,&st) < 0)
			   return (errno == ENOENT ? 0 : -1);
		  }
		else fd = mkdir (path,0700);

		if (fd >= 0) return (fd);
		if (errno != EEXIST) return (-1);
	 }

   return (-1);
}

/* temp path name */
static char *path = NULL;

/*
 * Create a temporary directory (only accessible by current process)
 * for use by tmp_name() function. You have to specify a temporary
 * directory ``tmpdir'' and a program name ``progname''. These strings
 * are used to create a directory of the form <tmpdir>/<progname><hash>.
 * Return 0 if successful, -1 if not. Check errno to see what error
 * occurred.
 */
int tmp_open_stub (const char *filename,int line,const char *function,const char *tmpdir,const char *progname)
{
   char *s;
   int fd;

   if ((path = bcat_stub (filename,line,function,tmpdir,"/",progname,padding,"/",padding,NULL)) == NULL)
	 return (-1);

   *(s = strrchr (path,'/')) = '\0';

   if ((fd = make_temp (path,1)) < 0)
	 {
		int saved = errno;
		mem_free (path);
		errno = saved;
		return (-1);
	 }

   strcat (path,"/");
   strcat (path,padding);

   close (fd);

   return (0);
}

/*
 * Remove the directory created by tmp_open(). It is the responsibility
 * of the programmer to ensure that the directory is empty (i.e. all
 * file/directories created with names generated by tmp_name() were
 * removed) by the time this function is called.
 */
void tmp_close ()
{
   if (path != NULL)
	 {
		char *s = strrchr (path,'/');
		*s = '\0';
		rmdir (path);
		mem_free (path);
	 }
}

/*
 * Create a name for a temporary file which is safe to use. Return
 * the filename if successful, NULL if not. Check errno to see what
 * error occurred. Remember to remove the file and free the returned
 * string when you're done.
 */
char *tmp_name_stub (const char *filename,int line,const char *function)
{
   char *s;

   if (make_temp (path,0) < 0)
	 return (NULL);

   if ((s = (char *) mem_alloc_stub (sizeof (char) * (strlen (path) + 1),filename,line,function)) == NULL)
	 {
		errno = ENOMEM;
		return (NULL);
	 }

   strcpy (s,path);

   return (s);
}

