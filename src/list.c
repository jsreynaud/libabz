
/*
 * Copyright (c) Abraham vd Merwe <abz@blio.com>
 * All rights reserved.
 *
 * This is based on code found on the Linux kernel mailinglist. The
 * original code had no copyright.
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

#include <abz/list.h>

void list_sort (struct list_head *head,int (*cmp) (struct list_head *a,struct list_head *b))
{
   struct list_head *p,*q,*e,*list,*tail,*oldhead;
   int insize,nmerges,psize,qsize,i;

   list = head->next;
   list_del (head);
   insize = 1;

   for (;;)
	 {
		p = oldhead = list;
		list = tail = NULL;
		nmerges = 0;

		while (p != NULL)
		  {
			 nmerges++;
			 q = p;
			 psize = 0;

			 for (i = 0; i < insize; i++)
			   {
				  psize++;
				  q = q->next == oldhead ? NULL : q->next;
				  if (q == NULL)
					break;
			   }

			 qsize = insize;

			 while (psize > 0 || (qsize > 0 && q != NULL))
			   {
				  if (!psize)
					{
					   e = q, q = q->next, qsize--;
					   if (q == oldhead)
						 q = NULL;
					}
				  else if (!qsize || !q)
					{
					   e = p, p = p->next, psize--;
					   if (p == oldhead)
						 p = NULL;
					}
				  else if (cmp (p,q) <= 0)
					{
					   e = p, p = p->next, psize--;
					   if (p == oldhead)
						 p = NULL;
					}
				  else
					{
					   e = q, q = q->next, qsize--;
					   if (q == oldhead)
						 q = NULL;
					}

				  if (tail)
					tail->next = e;
				  else
					list = e;

				  e->prev = tail, tail = e;
			   }

			 p = q;
		  }

		tail->next = list;
		list->prev = tail;

		if (nmerges <= 1)
		  break;

		insize <<= 1;
	 }

   head->next = list;
   head->prev = list->prev;
   list->prev->next = head;
   list->prev = head;
}

