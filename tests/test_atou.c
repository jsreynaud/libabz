
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
#include <stdint.h>

#include <debug/log.h>
#include <debug/memory.h>

#include <abz/atou8.h>
#include <abz/atou16.h>
#include <abz/atou32.h>
#include <abz/atou64.h>

static void fail (const char *fmt, ...)
{
   va_list ap;
   va_start (ap,fmt);
   if (fmt != NULL) log_vprintf (LOG_ERROR,fmt,ap);
   va_end (ap);
   exit (EXIT_FAILURE);
}

static void test_atou8_succeed (const char *str,uint8_t value)
{
   uint8_t tmp;

   if (atou8 (str,&tmp) < 0)
	 fail ("atou8(%s) failed\n",str);

   if (tmp != value)
	 fail ("mismatch: expected 0x%02x, got 0x%02x instead!\n",value,tmp);
}

static void test_atou16_succeed (const char *str,uint16_t value)
{
   uint16_t tmp;

   if (atou16 (str,&tmp) < 0)
	 fail ("atou16() failed\n");

   if (tmp != value)
	 fail ("mismatch: expected 0x%04x, got 0x%04x instead!\n",value,tmp);
}

static void test_atou32_succeed (const char *str,uint32_t value)
{
   uint32_t tmp;

   if (atou32 (str,&tmp) < 0)
	 fail ("atou32() failed\n");

   if (tmp != value)
	 fail ("mismatch: expected 0x%08x, got 0x%08x instead!\n",value,tmp);
}

static void test_atou64_succeed (const char *str,uint64_t value)
{
   uint64_t tmp;

   if (atou64 (str,&tmp) < 0)
	 fail ("atou64() failed\n");

   if (tmp != value)
	 fail ("mismatch [atou64(%s)]: expected 0x%016llx, got 0x%016llx instead!\n",str,value,tmp);
}

static void test_atou8_fail (const char *str)
{
   uint8_t tmp;
   if (!atou8 (str,&tmp))
	 fail ("atou8() succeeded for %s, but should've failed!\n",str);
}

static void test_atou16_fail (const char *str)
{
   uint16_t tmp;
   if (!atou16 (str,&tmp))
	 fail ("atou16() succeeded for %s, but should've failed!\n",str);
}

static void test_atou32_fail (const char *str)
{
   uint32_t tmp;
   if (!atou32 (str,&tmp))
	 fail ("atou32() succeeded for %s, but should've failed!\n",str);
}

static void test_atou64_fail (const char *str)
{
   uint64_t tmp;
   if (!atou64 (str,&tmp))
	 fail ("atou64() succeeded for %s, but should've failed!\n",str);
}

int main ()
{
   mem_open (NULL);
   log_open (NULL,LOG_NOISY,LOG_HAVE_COLORS | LOG_PRINT_FUNCTION);
   atexit (mem_close);
   atexit (log_close);

   test_atou8_succeed ("0",0);
   test_atou8_succeed ("0x00000000",0);
   test_atou8_succeed ("000000",0);
   test_atou8_succeed ("15",15);
   test_atou8_succeed ("0x1f",31);
   test_atou8_succeed ("077",63);
   test_atou8_succeed ("0xff",255);
   test_atou8_fail ("+0xff");
   test_atou8_fail ("-1");
   test_atou8_fail ("0644");
   test_atou8_fail ("256");
   test_atou8_fail (" 254");
   test_atou8_fail ("+ 254");

   log_printf (LOG_NORMAL,"atou8() seems to work ok on this platform\n");

   test_atou16_succeed ("0",0);
   test_atou16_succeed ("170",170);
   test_atou16_succeed ("0x7fff",32767);
   test_atou16_succeed ("065432",27418);
   test_atou16_succeed ("0xffff",65535);
   test_atou16_succeed ("65535",65535);
   test_atou16_fail ("-1");
   test_atou16_fail ("0654321");
   test_atou16_fail ("0x10000");
   test_atou16_fail ("65536");

   log_printf (LOG_NORMAL,"atou16() seems to work ok on this platform\n");

   test_atou32_succeed ("0",0);
   test_atou32_succeed ("170",170);
   test_atou32_succeed ("0x7fff",32767);
   test_atou32_succeed ("07777777777",1073741823);
   test_atou32_succeed ("0xffffffff",4294967295U);
   test_atou32_succeed ("4294967295",4294967295U);
   test_atou32_fail ("-1");
   test_atou32_fail ("077777777777");
   test_atou32_fail ("0x100000000");
   test_atou32_fail ("4294967296");

   log_printf (LOG_NORMAL,"atou32() seems to work ok on this platform\n");

   test_atou64_succeed ("0",0);
   test_atou64_succeed ("170",170);
   test_atou64_succeed ("0x7fff",32767);
   test_atou64_succeed ("0x7fffffff",2147483647);
   test_atou64_succeed ("0x7fffffffffffffff",9223372036854775807ULL);
   test_atou64_succeed ("01777777777777777777775",18446744073709551613ULL);
   test_atou64_succeed ("0xffffffffffffffff",18446744073709551615ULL);
   test_atou64_succeed ("18446744073709551613",18446744073709551613ULL);
   test_atou64_fail ("-1");
   test_atou64_fail ("184467440737095516160");
   test_atou64_fail ("18446744073709551616");
   test_atou64_fail ("0x100000000000000000");

   log_printf (LOG_NORMAL,"atou64() seems to work ok on this platform\n");

   exit (EXIT_SUCCESS);
}

