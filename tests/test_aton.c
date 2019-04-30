
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

#include <abz/aton.h>

#define test_aton_succeed(str,net,mask) test_aton_succeed_stub(__FILE__,__LINE__,__FUNCTION__,str,net,mask)
static void test_aton_succeed_stub (const char *file,int line,const char *function,
									const char *str,uint32_t net,uint32_t mask)
{
   struct network tmp;

   if (aton (&tmp,str) < 0)
	 {
		log_printf_stub (file,line,function,LOG_ERROR,
						 "aton() failed\n");
		exit (EXIT_FAILURE);
	 }

   if (tmp.address != net)
	 {
		log_printf_stub (file,line,function,LOG_ERROR,
						 "address mismatch: expected 0x%.8x, got 0x%.8x instead!\n",
						 net,tmp.address);
		exit (EXIT_FAILURE);
	 }

   if (tmp.netmask != mask)
	 {
		log_printf_stub (file,line,function,LOG_ERROR,
						 "netmask mismatch: expected 0x%.8x, got 0x%.8x instead!\n",
						 mask,tmp.netmask);
		exit (EXIT_FAILURE);
	 }
}

#define test_aton_fail(str) test_aton_fail_stub(__FILE__,__LINE__,__FUNCTION__,str)
static void test_aton_fail_stub (const char *file,int line,const char *function,
								 const char *str)
{
   struct network tmp;

   if (!aton (&tmp,str))
	 {
		log_printf_stub (file,line,function,LOG_ERROR,
						 "aton() succeeded for %s, but should've failed!\n",
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

   test_aton_succeed ("192.168.0.0/18",0x0000a8c0,0x00c0ffff);

#ifdef GETHOSTBYNAME
   test_aton_succeed ("oasis.frogfoot.net",0x3336d2a8,0xffffffff);
#endif	/* #ifdef GETHOSTBYNAME */

   test_aton_succeed ("255.255.255.255/32",0xffffffff,0xffffffff);
   test_aton_succeed ("0.0.0.0/0",0x00000000,0x00000000);
   test_aton_succeed ("127.0.0.1",0x0100007f,0xffffffff);
   test_aton_fail ("127.0.0.1/255.255.255.255");
   test_aton_fail ("oasis.");
   test_aton_fail ("");
   test_aton_fail (NULL);
   test_aton_fail ("/32");
   test_aton_fail ("192.168.1.0/");
   test_aton_fail ("192.168.0.0/10");
   test_aton_fail ("192.168.1.1/30");
   test_aton_fail ("a.b.c.d");
   test_aton_fail ("256.255.255.255");
   test_aton_fail ("0.0.0.0.0");
   test_aton_fail ("0.1.2");
   test_aton_fail ("1.2");
   test_aton_fail ("127");
   test_aton_fail ("1234567890");
   test_aton_fail ("-1.1.2.3");

   log_printf (LOG_NORMAL,"aton() seems to work ok on this platform\n");

   exit (EXIT_SUCCESS);
}

