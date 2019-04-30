
# -*- sh -*-

#  Copyright (c) Abraham vd Merwe <abz@blio.com>
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#  3. Neither the name of the author nor the names of other contributors
#     may be used to endorse or promote products derived from this software
#     without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
#  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

CPPFLAGS = -I../include
LDFLAGS = -L../src -static
LDLIBS = -labz -ldebug

# path to toplevel directory from here
TOPDIR = ..

# subdirectories (leave as is if there is no subdirectories)
DIR =

# names of object files
OBJ =

# program name (leave as is if there is no program)
PRG =

# library name (leave as is if there is no library)
LIB =

include $(TOPDIR)/paths.mk
include $(TOPDIR)/defs.mk
include $(TOPDIR)/vars.mk
include $(TOPDIR)/rules.mk

TEST = test_aton test_atou test_bcat	\
	test_bprintf test_mask test_line	\
	test_mkdirhier test_rmdirhier		\
	test_tmp test_trim test_file		\
	test_alive test_error test_parse	\
	test_pidfile test_atos test_atop	\
	test_atoa test_run test_list		\
	test_stream test_unblock test_tc	\
	test_iptables test_argv

all:: $(TEST)

test_aton: test_aton.o
test_atou: test_atou.o
test_bcat: test_bcat.o
test_bprintf: test_bprintf.o
test_mask: test_mask.o
test_line: test_line.o
test_mkdirhier: test_mkdirhier.o
test_rmdirhier: test_rmdirhier.o
test_tmp: test_tmp.o
test_trim: test_trim.o
test_file: test_file.o
test_alive: test_alive.o
test_error: test_error.o
test_parse: test_parse.o
test_pidfile: test_pidfile.o
test_atos: test_atos.o
test_atop: test_atop.o
test_atoa: test_atoa.o
test_run: test_run.o
test_list: test_list.o
test_stream: test_stream.o
test_unblock: test_unblock.o
test_tc: test_tc.o
test_iptables: test_iptables.o
test_argv: test_argv.o

clean::
	$(RM) $(TEST) $(addsuffix .o,$(TEST)) test_stream.txt test_unblock.fifo

