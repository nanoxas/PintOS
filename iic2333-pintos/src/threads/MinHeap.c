/*
 * MinHeap.c
 *
 *  Created on: Aug 30, 2015
 *      Author: nanoxas
 */
#include "threads/MinHeap.h"
#include "threads/thread.h"



minHeap initMinHeap(void){
	minHeap hp  ;
	hp.size=0;
	return hp;
}

void insertNode(minHeap* hp , node* elem){

	if(hp -> size){
		hp -> elem = realloc(hp -> elem, (hp->size+1)* sizeof(node));

	}
	else{
		hp -> elem = malloc(sizeof(node));

	}

	node nd = *(elem);
	int i= (hp->size)++;
	while(i && nd.data < hp->elem[PARENT(i)].data) {
		hp->elem[i] = hp->elem[PARENT(i)] ;
		i = PARENT(i) ;
	}
	hp->elem[i] = nd ;
}


void swap(node *n1, node *n2) {
	node temp = *n1 ;
	*n1 = *n2 ;
	*n2 = temp ;
}



void heapify(minHeap *hp, int i) {
	int smallest = (LCHILD(i) < hp->size && hp->elem[LCHILD(i)].data < hp->elem[i].data) ? LCHILD(i) : i ;



	if(RCHILD(i) < hp->size && hp->elem[RCHILD(i)].data < hp->elem[smallest].data) {
		smallest = RCHILD(i) ;
	}
	if(smallest != i) {
		swap(&(hp->elem[i]), &(hp->elem[smallest])) ;
		heapify(hp, smallest) ;
	}
}


void deleteNode(minHeap *hp) {
	if(hp->size) {
		hp->elem[0] = hp->elem[--(hp->size)] ;
		hp->elem = realloc(hp->elem, hp->size * sizeof(node)) ;
		heapify(hp, 0) ;
	} else {
		printf("\nMin Heap is empty!\n") ;
		free(hp->elem) ;
	}
}

void buildMinHeap(minHeap *hp, int *arr, int size) {
	int i ;

	// Insertion into the heap without violating the shape property
	for(i = 0; i < size; i++) {
		if(hp->size) {
			hp->elem = realloc(hp->elem, (hp->size + 1) * sizeof(node)) ;
		} else {
			hp->elem = malloc(sizeof(node)) ;
		}
		node nd ;
		nd.data = arr[i] ;
		hp->elem[(hp->size)++] = nd ;
	}

	// Making sure that heap property is also satisfied
	for(i = (hp->size - 1) / 2; i >= 0; i--) {
		heapify(hp, i) ;
	}
}

node getNextElement(minHeap* hp){


	//printf("%d\n", hp->size);
	if(hp-> size){
	node ret = *(hp->elem);
	deleteNode(hp);
	//printf("sali\n");
	return ret;
	}
	node* fail = malloc(sizeof(node));
	fail -> data=-1;
	return *fail;


}





