
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

#include <abz/atou64.h>

atou_base_t atou_base = ATOU_AUTO;

#define U64_MAX (uint64_t) -1

static inline int __octal (const char *head,const char *tail,uint64_t *u)
{
   uint64_t tmp,sum = 0,pow8 = 1,digits = 0;

   while (head <= --tail)
	 {
		if (digits++ == 23)
		  return (-1);

		if (*tail < '0' || *tail > '7')
		  return (-1);

		tmp = pow8 * (*tail - '0');

		if (sum > U64_MAX - tmp)
		  return (-1);

		sum += tmp;
		pow8 <<= 3;
	 }

   *u = sum;
   return (0);
}

static inline int __hex (const char *head,const char *tail,uint64_t *u)
{
   uint64_t tmp,sum = 0,pow16 = 1,digits = 0;

   while (head <= --tail)
	 {
		if (digits++ == 16)
		  return (-1);

		if (*tail >= '0' && *tail <= '9')
		  tmp = pow16 * (*tail - '0');
		else if (*tail >= 'a' && *tail <= 'f')
		  tmp = pow16 * (*tail - 'a' + 10);
		else if (*tail >= 'A' && *tail <= 'F')
		  tmp = pow16 * (*tail - 'A' + 10);
		else return (-1);

		if (sum > U64_MAX - tmp)
		  return (-1);

		sum += tmp;
		pow16 <<= 4;
	 }

   *u = sum;
   return (0);
}

int atou64 (const char *s,uint64_t *u)
{
   const char *d = s;
   uint64_t tmp,sum = 0,pow10 = 1,digits = 0;

   if (s == NULL || *s == '\0')
	 return (-1);

   while (*s != '\0') s++;

   if (atou_base == ATOU_AUTO || atou_base == ATOU_HEX)
	 if (s - d > 2 && d[0] == '0' && d[1] == 'x')
	   return (__hex (d + 2,s,u));

   if (atou_base == ATOU_AUTO || atou_base == ATOU_OCTAL)
	 if (s - d > 1 && d[0] == '0')
	   return (__octal (d + 1,s,u));

   if (atou_base == ATOU_AUTO || atou_base == ATOU_DECIMAL)
	 {
		while (d <= --s)
		  {
			 if (digits++ == 20)
			   return (-1);

			 if (*s < '0' || *s > '9')
			   return (-1);

			 tmp = pow10 * (*s - '0');

			 if (sum > U64_MAX - tmp)
			   return (-1);

			 sum += tmp;
			 pow10 *= 10;
		  }

		*u = sum;
	 }

   switch (atou_base)
	 {
	  case ATOU_OCTAL:
		return (__octal (d,s,u));

	  case ATOU_HEX:
		return (__hex (d,s,u));

	  case ATOU_AUTO:
	  case ATOU_DECIMAL:
		return (0);
	 }

   /* never reached - keep gcc happy */
   return (0);
}

