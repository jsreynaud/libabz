
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

#include <stdint.h>
#include <stddef.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#if defined(GETHOSTBYNAME) || defined(GETSERVBYNAME)
#include <netdb.h>
#endif	/* #if defined(GETHOSTBYNAME) || defined(GETSERVBYNAME) */

#include <abz/atou16.h>
#include <abz/atop.h>

#ifdef GETSERVBYNAME
static __inline__ int get_serv (in_port_t *port,const char *str)
{
   struct servent *serv;

   if ((serv = getservbyname (str,"tcp")) == NULL &&
	   (serv = getservbyname (str,"udp")) == NULL)
	 return (-1);

   *port = serv->s_port;

   return (0);
}
#endif	/* #ifdef GETSERVBYNAME */

static __inline__ int get_port (in_port_t *port,const char *str)
{
   uint16_t value;

   if (atou16 (str,&value))
	 return (-1);

   *port = htons (value);

   return (0);
}

int atop (in_port_t *port,const char *str)
{
   if (port == NULL || str == NULL)
	 {
		errno = EINVAL;
		return (-1);
	 }

   if (!get_port (port,str))
	 return (0);

   errno = EINVAL;

#ifdef GETSERVBYNAME
   if (!get_serv (port,str))
	 return (0);
#endif	/* #ifdef GETSERVBYNAME */

   return (-1);
}

