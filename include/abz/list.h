#ifndef ABZ_LIST_H
#define ABZ_LIST_H

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

struct list_head
{
   struct list_head *next,*prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

/*
 * Insert a new entry between two known consecutive entries. 
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static __inline__ void __list_add (struct list_head *entry,struct list_head *prev,struct list_head *next)
{
	next->prev = entry;
	entry->next = next;
	entry->prev = prev;
	prev->next = entry;
}

/*
 * Add a new entry after the specified head
 *
 *     entry    new entry to be added
 *     head     list head to add it after
 *
 * This is good for implementing stacks
 */
static __inline__ void list_add (struct list_head *entry,struct list_head *head)
{
   __list_add (entry,head,head->next);
}

/*
 * Add a new entry before the specified head
 *
 *     entry    new entry to be added
 *     head     list head to add it before
 *
 * This is good for implementing queues
 */
static __inline__ void list_add_tail (struct list_head *entry,struct list_head *head)
{
   __list_add (entry,head->prev,head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static __inline__ void __list_del (struct list_head *prev,struct list_head *next)
{
   next->prev = prev;
   prev->next = next;
}

/*
 * Delete an entry
 *
 *     entry    the element to delete
 *
 * list_empty on entry does not return true after this,
 * the entry is in an undefined state.
 */
static __inline__ void list_del (struct list_head *entry)
{
   __list_del (entry->prev,entry->next);
}

/*
 * Delete an entry and reinitialize it
 *
 *     entry    the element to delete
 */
static __inline__ void list_del_init (struct list_head *entry)
{
   __list_del (entry->prev,entry->next);
   INIT_LIST_HEAD (entry); 
}

/*
 * Test whether a list is empty
 */
static __inline__ int list_empty (struct list_head *head)
{
   return (head->next == head);
}

/*
 * Join two lists
 *
 *     list     the new list to add
 *     head     the place to add it in the first list
 */
static __inline__ void list_join (struct list_head *list,struct list_head *head)
{
   struct list_head *first = list->next;

   if (first != list)
	 {
		struct list_head *last = list->prev;
		struct list_head *at = head->next;

		first->prev = head;
		head->next = first;

		last->next = at;
		at->prev = last;
	}
}

/*
 * Sort a list
 *
 *     head     the &struct list_head pointer
 *     cmp      the compare function
 */
extern void list_sort (struct list_head *head,int (*cmp) (struct list_head *a,struct list_head *b));

/*
 * Get the struct for this entry
 *
 *     ptr      the &struct list_head pointer
 *     type     the type of the struct this is embedded in
 *     member   the name of the list_head within the struct
 */
#define list_entry(ptr,type,member) \
	((type *) ((char *) (ptr) - (unsigned long) (&((type *) 0)->member)))

/*
 * Iterate over a list
 *
 *     pos      the &struct list_head to use as a loop counter
 *     tmp      another &struct list_head to use as temporary storage
 *     head     the head for your list
 */
#define list_for_each(pos,tmp,head) \
	for (pos = (head)->next, tmp = pos->next; pos != (head); \
		pos = tmp, tmp = pos->next)

/*
 * Iterate over a list in reverse order
 *
 *     pos      the &struct list_head to use as a loop counter
 *     tmp      another &struct list_head to use as temporary storage
 *     head     the head for your list
 */
#define list_for_each_prev(pos,tmp,head) \
	for (pos = (head)->prev, tmp = pos->prev; pos != (head); \
		pos = tmp, tmp = pos->prev)

#endif	/* #ifndef ABZ_LIST_H */
