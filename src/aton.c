
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
#include <sys/types.h>
#include <netinet/in.h>

#include <abz/atou8.h>
#include <abz/aton.h>
#include <abz/atoa.h>

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

static __inline__ int get_network (struct network *net,const char *str)
{
   char *mask,addr[19];

   if (net == NULL || str == NULL || strlen (str) >= sizeof (addr))
	 {
		errno = EINVAL;
		return (-1);
	 }

   strcpy (addr,str);

   net->netmask = 0xffffffff;

   if ((mask = strchr (addr,'/')) != NULL)
	 {
		uint8_t tmp;

		*mask++ = '\0';

		if (atou8 (mask,&tmp) || tmp > 32)
		  return (-1);

		net->netmask = htonl (0xffffffff - ((1ULL << (32 - tmp)) - 1));
	 }

   if (get_addr (&net->address,addr))
	 return (-1);

   if ((net->address & net->netmask) != net->address)
	 {
		errno = EINVAL;
		return (-1);
	 }

   return (0);
}

static __inline__ int get_hostname (struct network *net,const char *str)
{
   if (net == NULL || str == NULL)
	 {
		errno = EINVAL;
		return (-1);
	 }

   net->netmask = 0xffffffff;

   return (atoa (&net->address,str));
}

/*
 * convert a string to a network structure. returns 0 if
 * successful, -1 if the string is not a valid network.
 */
int aton (struct network *net,const char *str)
{
   return (get_network (net,str) && get_hostname (net,str) ? -1 : 0);
}

