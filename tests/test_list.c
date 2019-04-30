
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

#include <stdlib.h>
#include <string.h>

#include <debug/log.h>
#include <debug/memory.h>

#include <abz/typedefs.h>
#include <abz/error.h>
#include <abz/list.h>

struct key
{
   int key;
   struct list_head node;
};

static void key_init (struct list_head *list)
{
   INIT_LIST_HEAD (list);
}

static void key_destroy (struct list_head *list)
{
   struct list_head *ctr,*tmp;
   struct key *entry;

   list_for_each (ctr,tmp,list)
	 {
		entry = list_entry (ctr,struct key,node);
		list_del (&entry->node);
		mem_free (entry);
	 }
}

static struct key *key_new (int key)
{
   struct key *entry;

   abz_clear_error ();

   if ((entry = mem_alloc (sizeof (*entry))) == NULL)
	 {
		abz_set_error ("failed to allocate memory: %m");
		return (NULL);
	 }

   memset (entry,0L,sizeof (*entry));
   entry->key = key;

   return (entry);
}

static int key_add (struct list_head *list,int key)
{
   struct key *entry;

   if ((entry = key_new (key)) == NULL)
	 return (-1);

   list_add (&entry->node,list);
   return (0);
}

static int key_add_tail (struct list_head *list,int key)
{
   struct key *entry;

   if ((entry = key_new (key)) == NULL)
	 return (-1);

   list_add_tail (&entry->node,list);
   return (0);
}

static int key_find (struct list_head *list,int key)
{
   struct list_head *ctr,*tmp;
   struct key *entry;

   if (list_empty (list))
	 return (0);

   list_for_each (ctr,tmp,list)
	 {
		entry = list_entry (ctr,struct key,node);

		if (entry->key == key)
		  return (1);
	 }

   return (0);
}

static void key_print (const char *prefix,struct list_head *list)
{
   struct list_head *ctr,*tmp;
   struct key *entry;

   log_printf (LOG_NORMAL,"%s [next]:",prefix);

   list_for_each (ctr,tmp,list)
	 {
		entry = list_entry (ctr,struct key,node);
		log_printf (LOG_NORMAL," %d",entry->key);
	 }

   log_putc (LOG_NORMAL,'\n');

   log_printf (LOG_NORMAL,"%s [prev]:",prefix);

   list_for_each_prev (ctr,tmp,list)
	 {
		entry = list_entry (ctr,struct key,node);
		log_printf (LOG_NORMAL," %d",entry->key);
	 }

   log_putc (LOG_NORMAL,'\n');
}

static void test_queue (void)
{
   int i;
   struct list_head head;

   key_init (&head);

   for (i = 0; i < 5; i++)
	 if (key_add_tail (&head,i + 1))
	   {
		  log_printf (LOG_ERROR,"%s\n",abz_get_error ());
		  key_destroy (&head);
		  exit (EXIT_FAILURE);
	   }

   key_print ("queue (1 -> 5)",&head);
   key_destroy (&head);
}

static void test_stack (void)
{
   int i;
   struct list_head head;

   key_init (&head);

   for (i = 0; i < 5; i++)
	 if (key_add (&head,i + 1))
	   {
		  log_printf (LOG_ERROR,"%s\n",abz_get_error ());
		  key_destroy (&head);
		  exit (EXIT_FAILURE);
	   }

   key_print ("stack (1 -> 5)",&head);
   key_destroy (&head);
}

static void test_find (void)
{
   int i;
   struct list_head head;

   key_init (&head);

   if (key_find (&head,0))
	 {
		log_printf (LOG_ERROR,"found something in empty list\n");
		exit (EXIT_FAILURE);
	 }

   for (i = 0; i < 5; i++)
	 {
		if (key_add (&head,i + 1))
		  {
			 log_printf (LOG_ERROR,"%s\n",abz_get_error ());
			 key_destroy (&head);
			 exit (EXIT_FAILURE);
		  }

		if (!key_find (&head,i + 1))
		  {
			 log_printf (LOG_ERROR,"tail not found: %d\n",i);
			 key_destroy (&head);
			 exit (EXIT_FAILURE);
		  }

		if (!key_find (&head,1))
		  {
			 log_printf (LOG_ERROR,"head not found: %d\n",i);
			 key_destroy (&head);
			 exit (EXIT_FAILURE);
		  }
	 }

   for (i = 0; i < 5; i++)
	 if (!key_find (&head,i + 1))
	   {
		  log_printf (LOG_ERROR,"key not found: %d\n",i + 1);
		  key_destroy (&head);
		  exit (EXIT_FAILURE);
	   }

   key_destroy (&head);
}

static int compare (struct list_head *a,struct list_head *b)
{
   struct key *k1 = list_entry (a,struct key,node),*k2 = list_entry (b,struct key,node);

   return (k1->key - k2->key);
}

static void test_sort (void)
{
   int i,key[] = { 1, 5, 3, 2, 4 };
   struct list_head head;

   key_init (&head);

   for (i = 0; i < ARRAYSIZE (key); i++)
	 if (key_add (&head,key[i]))
	   {
		  log_printf (LOG_ERROR,"%s\n",abz_get_error ());
		  key_destroy (&head);
		  exit (EXIT_FAILURE);
	   }

   list_sort (&head,compare);
   key_print ("sort",&head);
   key_destroy (&head);
}

static void test_join (void)
{
   int i;
   struct list_head a,b;

   key_init (&a);
   key_init (&b);

   for (i = 0; i < 5; i++)
	 if (key_add_tail (&a,i + 1) || key_add_tail (&b,i + 6))
	   {
		  log_printf (LOG_ERROR,"%s\n",abz_get_error ());
		  key_destroy (&a);
		  key_destroy (&b);
		  exit (EXIT_FAILURE);
	   }

   list_join (&a,&b);
   key_print ("join (1 -> 10)",&b);
   key_destroy (&b);
}

int main (void)
{
   mem_open (NULL);
   log_open (NULL,LOG_NOISY,LOG_HAVE_COLORS | LOG_PRINT_FUNCTION);
   atexit (mem_close);
   atexit (log_close);

   test_queue ();
   test_stack ();
   test_find ();
   test_sort ();
   test_join ();

   exit (EXIT_SUCCESS);
}

