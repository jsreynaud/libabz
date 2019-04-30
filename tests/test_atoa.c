
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

#include <abz/atoa.h>

#define test_atoa_succeed(str,addr) test_atoa_succeed_stub(__FILE__,__LINE__,__FUNCTION__,str,addr)
static void test_atoa_succeed_stub (const char *file,int line,const char *function,
									const char *str,uint32_t addr)
{
   in_addr_t tmp;

   if (atoa (&tmp,str))
	 {
		log_printf_stub (file,line,function,LOG_ERROR,
						 "atoa() failed\n");
		exit (EXIT_FAILURE);
	 }

   if (tmp != addr)
	 {
		log_printf_stub (file,line,function,LOG_ERROR,
						 "address mismatch: expected 0x%.8x, got 0x%.8x instead!\n",
						 addr,tmp);
		exit (EXIT_FAILURE);
	 }
}

#define test_atoa_fail(str) test_atoa_fail_stub(__FILE__,__LINE__,__FUNCTION__,str)
static void test_atoa_fail_stub (const char *file,int line,const char *function,
								 const char *str)
{
   in_addr_t tmp;

   if (!atoa (&tmp,str))
	 {
		log_printf_stub (file,line,function,LOG_ERROR,
						 "atoa() succeeded for %s, but should've failed!\n",
						 str);
		exit (EXIT_FAILURE);
	 }
}

int main ()
{
   mem_open (NULL);
   log_open (NULL,LOG_NOISY,LOG_HAVE_COLORS | LOG_PRINT_FUNCTION);
   atexit (mem_close);
   atexit (log_close);

#ifdef GETHOSTBYNAME
   test_atoa_succeed ("oasis.frogfoot.net",0x3336d2a8);
#endif	/* #ifdef GETHOSTBYNAME */

   test_atoa_succeed ("168.210.54.51",0x3336d2a8);
   test_atoa_succeed ("255.255.255.255",0xffffffff);
   test_atoa_succeed ("0.0.0.0",0x00000000);
   test_atoa_succeed ("127.0.0.1",0x0100007f);
   test_atoa_fail ("oasis.");
   test_atoa_fail ("");
   test_atoa_fail (NULL);
   test_atoa_fail ("a.b.c.d");
   test_atoa_fail ("256.255.255.255");
   test_atoa_fail ("0.0.0.0.0");
   test_atoa_fail ("0.1.2");
   test_atoa_fail ("1.2");
   test_atoa_fail ("127");
   test_atoa_fail ("1234567890");
   test_atoa_fail ("-1.1.2.3");

   log_printf (LOG_NORMAL,"atoa() seems to work ok on this platform\n");

   exit (EXIT_SUCCESS);
}

