/*
 * child.c
 *
 *  Created on: Oct 25, 2015
 *      Author: nanoxas
 */

#include "child.h"
#include <list.h>

child* add_child_thread(int tid){
	child * c = malloc(sizeof(child));
	c->tid = tid;
	c->loaded=0;
	c-> wait =0;
	c-> exit=0;
	list_push_back(&thread_current()-> child_list, &c->elem);
	c->parent = thread_current();
	return c;

}

child * get_child(int tid){

	struct thread* t = thread_current();
	struct list_elem* e;


	for (e = list_begin (&t->child_list); e != list_end (&t->child_list); e = list_next (e))
	{
		child *c = list_entry (e, child, elem);
		if (tid == c->tid)
		{
			return c;
		}
	}
	return NULL;
}

void remove_child(child* child){

	list_remove(&child->elem);
	free(child);

}
void remove_all_children(struct thread* cur){
	struct list_elem *e, *next;
	for (e = list_begin (&cur->child_list); e != list_end (&cur->child_list);
	       e = next)
	    {
	      child *cs = list_entry (e, child, elem);
	      next = list_remove (e);
	      remove_child (cs);
	    }
}
