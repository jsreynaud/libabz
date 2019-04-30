
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
#include <string.h>

#include <debug/memory.h>
#include <abz/error.h>
#include <abz/trim.h>
#include <abz/tokens.h>
#include <abz/bcat.h>

static void out_of_memory (void)
{
   abz_set_error ("failed to allocate memory: %m");
}

void tokens_destroy (struct tokens *tokens)
{
   int i;

   for (i = 0; i < tokens->argc; i++)
	 mem_free (tokens->argv[i]);

   if (tokens->argv != NULL)
	 mem_free (tokens->argv);
}

int tokens_parse_stub (const char *filename,int line,const char *function,
					   struct tokens *tokens,const char *str)
{
   int i,gotstr = 0;
   char *prev = (char *) str;
   void *ptr;

   tokens->argv = NULL;
   tokens->argc = 0;

   do
	 {
		if ((ptr = mem_realloc_stub (tokens->argv,(tokens->argc + 1) * sizeof (char *),
									 filename,line,function)) == NULL)
		  {
			 out_of_memory ();
			 tokens_destroy (tokens);
			 return (-1);
		  }

		tokens->argv = ptr;

		if ((tokens->argv[tokens->argc] = bcat_stub (filename,line,function,
													 prev,NULL)) == NULL)
		  {
			 out_of_memory ();
			 tokens_destroy (tokens);
			 return (-1);
		  }

		ltrim (tokens->argv[tokens->argc]);

		if (!gotstr && tokens->argv[tokens->argc][0] != '"')
		  {
			 if ((prev = strchr (tokens->argv[tokens->argc],' ')) != NULL ||
				 (prev = strchr (tokens->argv[tokens->argc],'"')) != NULL ||
				 (prev = strchr (tokens->argv[tokens->argc],'\t')) != NULL)
			   {
				  gotstr = *prev == '"';
				  *prev++ = '\0';
				  ltrim (prev);
			   }
		  }
		else
		  {
			 char *s = tokens->argv[tokens->argc],*d = s + 1;

			 do *s++ = *d; while (*d++ != '\0');
			 s = tokens->argv[tokens->argc];
			 while (*s != '"' && *s != '\0') s++;

			 if (*s == '\0')
			   {
				  abz_set_error ("missing \" at end of string");
				  tokens->argc++;
				  tokens_destroy (tokens);
				  return (-1);
			   }

			 *s++ = '\0';
			 prev = *s != '\0' ? s : NULL;
			 gotstr = 0;
		  }

		tokens->argc++;
	 } while (prev != NULL);

   for (i = 0; i < tokens->argc; i++)
	 if ((ptr = mem_realloc_stub (tokens->argv[i],strlen (tokens->argv[i]) + 1,
								  filename,line,function)) != NULL)
	   tokens->argv[i] = ptr;

   return (0);
}

