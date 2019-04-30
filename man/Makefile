
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

MAN = aton.3abz atou8.3abz atou16.3abz atou32.3abz atou64.3abz	\
	abz_set_error.3abz bcat.3abz bprintf.3abz getline.3abz		\
	trim.3abz alive.3abz mkdirhier.3abz rmdirhier.3abz			\
	tmp_name.3abz getfile.3abz putfile.3abz contigmask.3abz		\
	sanitize.3abz tokens_parse.3abz pidfile.3abz atos.3abz		\
	atop.3abz atoa.3abz list.3abz run.3abz stream_open.3abz		\
	unblock.3abz batch_create.3abz iptables_create.3abz			\
	tc_create.3abz strtoargv.3abz

LIST = LIST_HEAD_INIT.3abz.gz LIST_HEAD.3abz.gz					\
	INIT_LIST_HEAD.3abz.gz list_add.3abz.gz						\
	list_add_tail.3abz.gz list_del.3abz.gz						\
	list_del_init.3abz.gz list_empty.3abz.gz					\
	list_join.3abz.gz list_sort.3abz.gz list_entry.3abz.gz		\
	list_for_each.3abz.gz list_for_each_prev.3abz.gz

STREAM = stream_read.3abz.gz stream_getline.3abz.gz				\
	stream_write.3abz.gz stream_close.3abz.gz

BATCH = batch_add.3abz.gz batch_append.3abz.gz					\
	batch_destroy.3abz.gz batch_commit.3abz.gz

IPTABLES = iptables_add.3abz.gz iptables_append.3abz.gz			\
	iptables_destroy.3abz.gz iptables_commit.3abz.gz

TC = tc_add.3abz.gz tc_append.3abz.gz tc_destroy.3abz.gz		\
	tc_commit.3abz.gz

ARGV = vtoargv.3abz.gz toargv.3abz.gz freeargv.3abz.gz			\
	argvtostr.3abz.gz

MAN_LNK = vbprintf.3abz.gz tmp_open.3abz.gz tmp_close.3abz.gz	\
	ltrim.3abz.gz rtrim.3abz.gz abz_cat_error.3abz.gz			\
	abz_clear_error.3abz.gz abz_get_error.3abz.gz				\
	tokens_destroy.3abz $(LIST) vrun.3abz.gz $(STREAM)			\
	block.3abz.gz $(BATCH) $(IPTABLES) $(TC) $(ARGV)

install::
	$(INSTALL) -d $(mandir)/man3
	set -e; for F in $(MAN); do $(INSTALL) -c -m 0644 $$F $(mandir)/man3 && gzip -9 $(mandir)/man3/$$F; done
	$(LN) bprintf.3abz.gz $(mandir)/man3/vbprintf.3abz.gz
	$(LN) tmp_name.3abz.gz $(mandir)/man3/tmp_open.3abz.gz
	$(LN) tmp_name.3abz.gz $(mandir)/man3/tmp_close.3abz.gz
	$(LN) trim.3abz.gz $(mandir)/man3/ltrim.3abz.gz
	$(LN) trim.3abz.gz $(mandir)/man3/rtrim.3abz.gz
	$(LN) abz_set_error.3abz.gz $(mandir)/man3/abz_cat_error.3abz.gz
	$(LN) abz_set_error.3abz.gz $(mandir)/man3/abz_get_error.3abz.gz
	$(LN) abz_set_error.3abz.gz $(mandir)/man3/abz_clear_error.3abz.gz
	$(LN) tokens_parse.3abz.gz $(mandir)/man3/tokens_destroy.3abz.gz
	set -e; for F in $(LIST); do $(LN) list.3abz.gz $(mandir)/man3/$$F; done
	$(LN) run.3abz.gz $(mandir)/man3/vrun.3abz.gz
	set -e; for F in $(STREAM); do $(LN) stream_open.3abz.gz $(mandir)/man3/$$F; done
	$(LN) unblock.3abz.gz $(mandir)/man3/block.3abz.gz
	set -e; for F in $(BATCH); do $(LN) batch_create.3abz.gz $(mandir)/man3/$$F; done
	set -e; for F in $(IPTABLES); do $(LN) iptables_create.3abz.gz $(mandir)/man3/$$F; done
	set -e; for F in $(TC); do $(LN) tc_create.3abz.gz $(mandir)/man3/$$F; done
	set -e; for F in $(ARGV); do $(LN) strtoargv.3abz.gz $(mandir)/man3/$$F; done

uninstall::
	set -e; for F in $(MAN_LNK) $(MAN); do $(RM) $(mandir)/man3/$$F; done

