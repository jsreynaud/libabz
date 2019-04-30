
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

#include <abz/atop.h>

#define test_atop_succeed(str,port) test_atop_succeed_stub(__FILE__,__LINE__,__FUNCTION__,str,port)
static void test_atop_succeed_stub (const char *file,int line,const char *function,
									const char *str,uint16_t port)
{
   in_port_t __port;

   if (atop (&__port,str))
	 {
		log_printf_stub (file,line,function,LOG_ERROR,
						 "atop() failed\n");
		exit (EXIT_FAILURE);
	 }

   if (__port != port)
	 {
		log_printf_stub (file,line,function,LOG_ERROR,
						 "port mismatch: expected 0x%.4x, got 0x%.4x instead!\n",
						 port,__port);
		exit (EXIT_FAILURE);
	 }
}

#define test_atop_fail(str) test_atop_fail_stub(__FILE__,__LINE__,__FUNCTION__,str)
static void test_atop_fail_stub (const char *file,int line,const char *function,
								 const char *str)
{
   in_port_t __port;

   if (!atop (&__port,str))
	 {
		log_printf_stub (file,line,function,LOG_ERROR,
						 "atop() succeeded for %s, but should've failed!\n",
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

   test_atop_succeed ("65535",65535);
   test_atop_succeed ("0",0);

#ifdef GETSERVBYNAME
   test_atop_succeed ("snmp",0xa100);
   test_atop_succeed ("www",0x5000);
   test_atop_succeed ("tfido",0x11eb);
#endif	/* GETSERVBYNAME */

   test_atop_fail ("65536");
   test_atop_fail ("-1");
   test_atop_fail ("");
   test_atop_fail (NULL);
   test_atop_fail ("wwww");
   test_atop_fail ("a.a.a.a");

   log_printf (LOG_NORMAL,"atop() seems to work ok on this platform\n");

   exit (EXIT_SUCCESS);
}

