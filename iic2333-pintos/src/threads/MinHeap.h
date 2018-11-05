/*
 * MinHeap.h
 *
 *  Created on: Aug 30, 2015
 *      Author: nanoxas
 */

#include<list.h>

#ifndef MINHEAP_H_
#define MINHEAP_H_



#endif /* MINHEAP_H_ */


#define LCHILD(x) 2*x+1
#define RCHILD(x) 2*x +2
#define PARENT(x) x/2

typedef struct node{
	int data;
	struct thread* elem;

} node;

typedef struct minHeap {
	int size;
	node* elem;
} minHeap;


minHeap initMinHeap(void);
void insertNode(minHeap* , node* );
void swap(node* ,node*);
void heapify(minHeap*, int);
void deleteNode(minHeap*);
void buildMinHeap(minHeap*, int*, int);
node getNextElement(minHeap*);






