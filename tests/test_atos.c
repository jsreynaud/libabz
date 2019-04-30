
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

#include <abz/atos.h>

#define test_atos_succeed(str,net,mask) test_atos_succeed_stub(__FILE__,__LINE__,__FUNCTION__,str,net,mask)
static void test_atos_succeed_stub (const char *file,int line,const char *function,
									const char *str,uint32_t addr,uint16_t port)
{
   struct sockaddr_in tmp;

   if (atos (&tmp,str))
	 {
		log_printf_stub (file,line,function,LOG_ERROR,
						 "atos() failed\n");
		exit (EXIT_FAILURE);
	 }

   if (tmp.sin_addr.s_addr != addr)
	 {
		log_printf_stub (file,line,function,LOG_ERROR,
						 "address mismatch: expected 0x%.8x, got 0x%.8x instead!\n",
						 addr,tmp.sin_addr.s_addr);
		exit (EXIT_FAILURE);
	 }

   if (tmp.sin_port != port)
	 {
		log_printf_stub (file,line,function,LOG_ERROR,
						 "port mismatch: expected 0x%.4x, got 0x%.4x instead!\n",
						 port,tmp.sin_port);
		exit (EXIT_FAILURE);
	 }
}

#define test_atos_fail(str) test_atos_fail_stub(__FILE__,__LINE__,__FUNCTION__,str)
static void test_atos_fail_stub (const char *file,int line,const char *function,
								 const char *str)
{
   struct sockaddr_in tmp;

   if (!atos (&tmp,str))
	 {
		log_printf_stub (file,line,function,LOG_ERROR,
						 "atos() succeeded for %s, but should've failed!\n",
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
   test_atos_succeed ("oasis.frogfoot.net",0x3336d2a8,0);
   test_atos_succeed ("oasis.frogfoot.net:12345",0x3336d2a8,0x3930);
#endif	/* #ifdef GETHOSTBYNAME */

#ifdef GETSERVBYNAME
   test_atos_succeed ("168.210.54.51:snmp",0x3336d2a8,0xa100);
#endif	/* #ifdef GETSERVBYNAME */

   test_atos_succeed ("168.210.54.51:169",0x3336d2a8,0xa900);
   test_atos_succeed ("168.210.54.51",0x3336d2a8,0);
   test_atos_succeed ("255.255.255.255",0xffffffff,0);
   test_atos_succeed ("0.0.0.0",0x00000000,0);
   test_atos_succeed ("127.0.0.1",0x0100007f,0);
   test_atos_fail ("127.0.0.1/255.255.255.255");
   test_atos_fail ("oasis.");
   test_atos_fail ("");
   test_atos_fail (NULL);
   test_atos_fail ("a.b.c.d");
   test_atos_fail ("256.255.255.255");
   test_atos_fail ("0.0.0.0.0");
   test_atos_fail ("0.1.2");
   test_atos_fail ("1.2");
   test_atos_fail ("127");
   test_atos_fail ("1234567890");
   test_atos_fail ("-1.1.2.3");
   test_atos_fail ("127.0.0.1:");
   test_atos_fail (":169");

   log_printf (LOG_NORMAL,"atos() seems to work ok on this platform\n");

   exit (EXIT_SUCCESS);
}

