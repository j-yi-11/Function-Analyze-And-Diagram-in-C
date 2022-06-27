#ifndef LIST_H
#define LIST_H
struct list_node{
	void *data;//由于是通用链表，所以用void*来指向数据 
	struct list_node *next, *prev;//这里采用双向链表 
};
typedef struct list_node ListNode;
typedef struct{
	ListNode *head, *tail;
}List;
List *NewList();
void ListInsert(List *list, void *data, int size);//向list内插入data数据，大小为size 
#endif
