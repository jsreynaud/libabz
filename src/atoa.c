
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
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#if defined(GETHOSTBYNAME) || defined(GETSERVBYNAME)
#include <netdb.h>
#endif	/* #if defined(GETHOSTBYNAME) || defined(GETSERVBYNAME) */

#include <abz/atou8.h>
#include <abz/atoa.h>

#ifdef GETHOSTBYNAME
static __inline__ int get_host (in_addr_t *addr,const char *str)
{
   struct hostent *host;
   struct in_addr tmp;

   if ((host = gethostbyname (str)) == NULL)
	 return (-1);

   memcpy (&tmp,(struct in_addr *) host->h_addr,sizeof (struct in_addr));

   if ((host = gethostbyaddr (&tmp,sizeof (struct in_addr),AF_INET)) == NULL)
	 return (-1);

   *addr = tmp.s_addr;

   return (0);
}
#endif	/* #ifdef GETHOSTBYNAME */

static char *next (uint8_t *result,char *str)
{
   char *s;

   if ((s = strchr (str,'.')) == NULL)
	 return (NULL);

   *s++ = '\0';

   if (atou8 (str,result))
	 return (NULL);

   return (s);
}

static __inline__ int get_addr (in_addr_t *addr,const char *str)
{
   uint8_t tmp;
   char *s,buf[16];

   if (strlen (str) >= sizeof (buf))
	 return (-1);

   strcpy (buf,str);

   if ((s = next (&tmp,buf)) == NULL)
	 return (-1);

   *addr = tmp;

   if ((s = next (&tmp,s)) == NULL)
	 return (-1);

   *addr |= tmp << 8;

   if ((s = next (&tmp,s)) == NULL)
	 return (-1);

   *addr |= tmp << 16;

   if (atou8 (s,&tmp))
	 return (-1);

   *addr |= tmp << 24;

   return (0);
}

int atoa (in_addr_t *addr,const char *str)
{
   if (addr == NULL || str == NULL)
	 {
		errno = EINVAL;
		return (-1);
	 }

   if (!get_addr (addr,str))
	 return (0);

   errno = EINVAL;

#ifdef GETSERVBYNAME
   if (!get_host (addr,str))
	 return (0);
#endif	/* #ifdef GETSERVBYNAME */

   return (-1);
}

