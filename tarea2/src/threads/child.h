/*
 * child.h
 *
 *  Created on: Oct 25, 2015
 *      Author: nanoxas
 */

#ifndef CHILD_H_
#define CHILD_H_

typedef struct child
{
	int tid;
	int wait;
	int exit;
	struct list_elem elem;
	int loaded;
	int exitcode;
	struct thread* parent;


} child;


child* add_child_thread(int);
child* get_child(int);
void remove_child(child*);
void delete_all();




#endif /* CHILD_H_ */
