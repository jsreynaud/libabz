
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

#include <sys/types.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>

#include <debug/memory.h>

#include <abz/typedefs.h>
#include <abz/error.h>
#include <abz/stream.h>

/* initial line length (must be power of 2) */
#define LINELEN 64

struct line
{
   size_t offset;
   size_t length;
   char *buf;
};

static void out_of_memory (void)
{
   abz_set_error ("failed to allocate memory: %m");
}

void stream_open (struct stream *stream,int fd)
{
   memset (stream,0L,sizeof (struct stream));
   stream->fd = fd;
}

ssize_t stream_read (struct stream *stream,void *buf,size_t count)
{
   /* make sure that buffer is large enough */
   if (count > stream->length)
	 {
		void *ptr;
		size_t length = MAX (count,stream->length + LINELEN);

		if ((ptr = mem_realloc (stream->buf,length)) == NULL)
		  {
			 out_of_memory ();
			 return (-1);
		  }

		stream->buf = ptr;
		stream->length = length;
	 }

   /* make sure that buffer contain enough data */
   if (count > stream->offset)
	 {
		ssize_t result;

		while ((result = read (stream->fd,
							   stream->buf + stream->offset,
							   stream->length - stream->offset)) < 0)
		  if (errno != EAGAIN && errno != EINTR)
			{
			   abz_set_error ("read: %m");
			   return (-1);
			}

		stream->offset += result;

		if (count > stream->offset)
		  count = stream->offset;
	 }

   /* copy requested data, if any */
   if (count)
	 {
		memcpy (buf,stream->buf,count);
		stream->offset -= count;
		memmove (stream->buf,stream->buf + count,stream->offset);
	 }

   return (count);
}

static __inline__ int lcat (struct line *line,char c)
{
   size_t length = (line->length + LINELEN) & ~(LINELEN - 1);

   if (line->buf == NULL && (line->buf = mem_alloc (length)) == NULL)
	 return (-1);

   if (line->length >= length - 1)
	 {
		void *ptr;

		if ((ptr = mem_realloc (line->buf,length + LINELEN)) == NULL)
		  return (-1);

		line->buf = ptr;
	 }

   line->buf[line->length++] = c;

   return (0);
}

ssize_t stream_getline (struct stream *stream,char **buf)
{
   char c;
   ssize_t result;
   struct line line;

   memset (&line,0L,sizeof (struct line));
   *buf = NULL;

   while ((result = stream_read (stream,&c,1)) > 0)
	 {
		if (c == '\0')
		  {
			 abz_set_error ("invalid ascii character: 0x%02x",c);
			 return (-1);
		  }

		if (lcat (&line,c))
		  {
			 out_of_memory ();
			 return (-1);
		  }

		if (c == '\n')
		  break;
	 }

   if (result < 0)
	 return (-1);

   if (line.length)
	 {
		line.buf[line.length] = '\0';
		*buf = line.buf;
	 }

   return (line.length);
}

ssize_t stream_write (struct stream *stream,const char *buf,size_t count)
{
   ssize_t result;

   while ((result = write (stream->fd,buf,count)) < 0)
	 if (errno != EAGAIN && errno != EINTR)
	   {
		  abz_set_error ("write: %m");
		  return (-1);
	   }

   return (result);
}

void stream_close (struct stream *stream)
{
   close (stream->fd);

   if (stream->buf != NULL)
	 mem_free (stream->buf);

   memset (stream,0L,sizeof (struct stream));
}

