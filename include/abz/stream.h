#ifndef ABZ_STREAM_H
#define ABZ_STREAM_H

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

struct stream
{
   int fd;
   char *buf;
   size_t offset;
   size_t length;
};

/*
 * Initialize stream structure.
 *
 *     stream    Pointer to uninitialized stream structure
 *     fd        Open file descriptor
 *
 * After this function is called, the file descriptor should
 * not be touched.
 */
extern void stream_open (struct stream *stream,int fd);

/*
 * Read data from the file descriptor. Reads are buffered.
 *
 *     stream    Pointer to initialized stream structure
 *     buf       Pointer to buffer where data should be stored
 *     count     Length of buffer
 *
 * On success, the number of bytes read is returned (zero
 * indicates nothing was read).  On error, -1 is returned,
 * and the error message is saved using abz_set_error().
 *
 * If count is zero and the file descriptor refers to a socket, it
 * should be assumed that the remote side terminated the connection.
 *
 * It is the responsibility of the caller to make sure that the
 * file descriptor is readable before calling this function.
 */
extern ssize_t stream_read (struct stream *stream,void *buf,size_t count);

/*
 * Read a line of data, including the trailing '\n', from the
 * file descriptor. Reads are buffered.
 *
 *     stream    Pointer to initialized stream structure
 *     buf       Pointer to buffer where ASCIIZ string should be stored
 *
 * On success, the number of bytes read is returned (zero
 * indicates nothing was read). On error or if a '\0' character
 * is read, -1 is returned and the error message is saved using
 * abz_set_error().
 *
 * If count is zero and the file descriptor refers to a socket, it
 * should be assumed that the remote side terminated the connection.
 */
extern ssize_t stream_getline (struct stream *stream,char **buf);

/*
 * Write data to the file descriptor. Writes are not buffered.
 *
 *     stream    Pointer to initialized stream structure
 *     buf       Pointer to buffer containing data to be written
 *     count     Length of buffer
 *
 * On success, the number of bytes written is returned (zero
 * indicates nothing was written).  On error, -1 is returned,
 * and the error message is saved using abz_set_error().
 *
 * If count is zero and the file descriptor refers to a socket, it
 * should be assumed that the remote side terminated the connection.
 *
 * It is the responsibility of the caller to make sure that the
 * file descriptor is writable before calling this function.
 */
extern ssize_t stream_write (struct stream *stream,const char *buf,size_t count);

/*
 * Free memory allocated for the stream structure and close
 * the file descriptor.
 *
 *     stream    Pointer to initialized stream structure
 */
extern void stream_close (struct stream *stream);

#endif	/* #ifndef ABZ_STREAM_H */
