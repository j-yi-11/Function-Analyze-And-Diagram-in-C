#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
List *NewList(){
	List *newlist = (List*)malloc(sizeof(List));
	newlist->head = NULL;
	newlist->tail = NULL;
	return newlist;
}
void ListInsert(List *list, void *data, int size){
	ListNode *newnode = NULL;
	newnode = (ListNode *)malloc(sizeof(ListNode));
	newnode->data = malloc(size);
	memcpy(newnode->data, data, size);
	newnode->prev = list->tail;
	newnode->next = NULL;
	if(list->tail != NULL)
	  list->tail->next = newnode;
	list->tail = newnode;
	if(list->head == NULL)
	  list->head = newnode;
}

