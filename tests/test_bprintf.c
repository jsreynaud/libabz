
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

#include <abz/bprintf.h>

static void fail (const char *fmt, ...)
{
   va_list ap;
   va_start (ap,fmt);
   if (fmt != NULL) log_vprintf (LOG_ERROR,fmt,ap);
   va_end (ap);
   exit (EXIT_FAILURE);
}

#define test_bprintf(answer,fmt,args...) do {														\
		char *buf = bprintf (fmt,##args);															\
		if (buf == NULL) fail ("bprintf(): %s\n",strerror (errno));									\
		if (strcmp (buf,answer)) {																	\
			log_printf (LOG_ERROR,"mismatch: expected ``%s'', got ``%s'' instead!\n",answer,buf);	\
			mem_free (buf);																			\
			fail (NULL);																			\
		}																							\
		mem_free (buf);																				\
	} while (0)

int main ()
{
   mem_open (NULL);
   log_open (NULL,LOG_NOISY,LOG_HAVE_COLORS | LOG_PRINT_FUNCTION);
   atexit (mem_close);
   atexit (log_close);

   /*
	* no need to test vbprintf() since its getting called by
	* bprintf() anyway...
	*/

   test_bprintf ("1, 1\n1, 2\n1, 3\n","1, 1\n1, 2%c1, 3\n",10);
   test_bprintf ("hello brave new world\n","hello brave new world\n");
   test_bprintf ("hello world!\n","hello %s%c%c","world",'!','\n');
   test_bprintf ("-1 0 1\n","%d %u %x\n",-1,0,1);

   log_printf (LOG_NORMAL,"bprintf() seems to work ok on this platform\n");

   exit (EXIT_SUCCESS);
}

