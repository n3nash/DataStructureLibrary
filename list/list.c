#include "list.h"
#include <stdio.h>
#include <stdlib.h>

int __init(list_t *list)
{
	if(!list){
		printf("Cannot initialize a NULL list\n");
		return -1;
	}
	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	list->allocator = (struct __list_allocator_t *)malloc(sizeof (struct __list_allocator_t));
	if(!list->allocator){
		printf("Couldn't initialize new empty list: out of memory\n");
		return -1;
	}
	return 0;
}


void init(list_t *list)
{
	if(!__init(list)){
		list->allocator->alloc = malloc;
		list->allocator->dealloc = free;
	}

}


void init_with_allocator(list_t *list, void *(*allocator)(unsigned long), void (*deallocator)(void*))
{
	
	if(!__init(list)){
		list->allocator->alloc = allocator;
		list->allocator->dealloc = deallocator;
	}
}

void set_allocator(list_t *list, void *(*allocator)(unsigned long), void (*deallocator)(void*))
{
	if(!list){
		init_with_allocator(list, allocator, deallocator);
	} else{
		if(list->allocator){
			list->allocator->alloc = allocator;
			list->allocator->dealloc = deallocator;
		} else{
			list->allocator = (struct __list_allocator_t *)malloc(sizeof (struct __list_allocator_t));
			list->allocator->alloc = allocator;
			list->allocator->dealloc = deallocator;
		}
	}
}

struct __list_node_t *__mknode(list_t *list, void *data)
{
	//should we just throw an error or make a new list?
	if(!list){
		init(list);
	}
	//make a new node
	struct __list_node_t *newnode = list->allocator->alloc(sizeof (struct __list_node_t));
	if(!newnode){
		printf("Couldn't allocate a new list node for list: out of memory\n");
		return NULL;
	}
	newnode->data = data;
	return newnode;
}

void add_front(list_t *list, void *data)
{
	struct __list_node_t *newnode = __mknode(list, data);
	if(!newnode){
		return;
	}
	if(!list->head){ //no front node. so newnode is head and tail and list should have a size of 1
		if(list->size || list->tail){
			printf("List has been corrupted: cannot add node\n");
			return;
		}
		list->head = newnode;
		newnode->prev = NULL;
		newnode->next = NULL;
		list->tail = newnode;
		list->size++;
	} else{ //there is already a front node, push it back
		newnode->prev = NULL;
		newnode->next = list->head;
		list->head->prev = newnode;
		list->head = newnode;
		list->size++;
	}

}

void add_back(list_t *list, void *data)
{
	struct __list_node_t *newnode = __mknode(list, data);
	if(!newnode){
		return;
	}
	if(!list->tail){
		if(list->size || list->head){
			printf("List has been corrupted: cannot add node\n");
			return;
		}
		list->tail = newnode;
		newnode->next = NULL;
		newnode->prev = NULL;
		list->head = newnode;
		list->size++;
	} else{
		newnode->next = NULL;
		newnode->prev = list->tail;
		list->tail->next = newnode;
		list->tail = newnode;
		list->size++;
	}

}


void add_at(list_t *list, void *data, int pos)
{
	if(!list){
		init(list);
	}
	if(pos == 0){
		add_front(list, data);
	} else if(pos > list->size - 1 || pos < 0){
		printf("Cannot add node at pos %d as size is %d", pos, list->size);
		return;
	} else if(pos == list->size - 1){
		add_back(list, data);
	} else{
		//no head or tail to worry about
		struct __list_node_t *newnode = __mknode(list, data);
		//now traverse to pos
		struct __list_node_t *trav = list->head;
		if(!trav){
			printf("Cannot add node: list has been corrupted\n");
			return;
		}
		int idx;
		for(idx = 0; idx != pos; idx++, trav = trav->next);
		//now we are at the position, trav has current node at position
		//the node at pos is shifted one in the list
		if(trav->prev){
			newnode->next = trav;
			newnode->prev = trav->prev;
			newnode->prev->next = newnode;
			trav->prev = newnode;
			list->size++;
		} else{ //no prev implies head, but we checked that, so fail
			printf("Cannot add node: list has been corrupted\n");
		}
			
	}
}


void __remove(list_t *list, struct __list_node_t *trav)
{
	if(trav->next){
		if(trav->prev){
			//inner node, can just do a swap
			trav->prev->next = trav->next;
			trav->next->prev = trav->prev;
			trav->prev = NULL;
			trav->next = NULL;
			list->allocator->dealloc(trav);
			trav = NULL;
			list->size--;
		} else{
			//if next exists, but prev doesnt, assume we are removing the head
			trav->next->prev = NULL;
			list->head = trav->next;
			trav->next = NULL;
			list->allocator->dealloc(trav);
			list->size--;
		}
	} else if(trav->prev){
		//removing the tail
		trav->prev->next = NULL;
		list->tail = trav->prev;
		trav->prev = NULL;
		list->allocator->dealloc(trav);
		list->size--;
	} else{
		printf("Couldn't remove node: list is corrupted\n");
		return;
	}
}

void remove_data(list_t *list, void *data, int (*cmp_fn)(void*, void*))
{
	if(!list || list->size == 0){
		printf("Cannot remove from empty list");
	} else{
		struct __list_node_t *trav;
		for(trav = list->head; trav; trav = trav->next){
			if(cmp_fn){
				if(!cmp_fn(data, trav->data)){
					__remove(list, trav);
					return;
				}
			} else if(data == trav->data){
					__remove(list, trav);
					return;
				}
			}
		}
	}


void remove_all(list_t *list, void *data, int (*cmp_fn)(void*, void*))
{
	if(!list || list->size == 0){
		printf("Cannot remove from empty list");
	} else{
		struct __list_node_t *trav;
		for(trav = list->head; trav; trav = trav->next){
			if(cmp_fn){
				if(!cmp_fn(data, trav->data)){
					__remove(list, trav);
				}
			} else if(data == trav->data){
					__remove(list, trav);
				}
			}
		}
	}

void remove_at(list_t *list, int pos)
{
	if(!list || list->size == 0){
		printf("Cannot remove from empty list");
	} else if(pos < 0 || pos > list->size - 1){
		printf("Cannot remove node at position %d: list too small or invalid position", pos);
	} else{
		struct __list_node_t *trav = list->head;
		int idx;
		for(idx = 0; idx != pos; idx++, trav = trav->next);
		__remove(list, trav);
	}
}

void *get(list_t *list, int pos)
{
	if(!list || list->size == 0){
		return NULL;
	} else if(pos < 0 || pos > list->size - 1){
		printf("Cannot get node at position %d: list too small or invalid position", pos);
		return NULL;
	} else{
		struct __list_node_t *trav = list->head;
		int idx;
		for(idx = 0; idx != pos; idx++, trav = trav->next);
		return trav == NULL ? NULL : trav->data;	
	}
}

void * __forward_nxt_iter_fn(forward_iterator_t *it)
{
	list_t *list = (list_t *)it->structure;
	if(!it->curr_location){
		return NULL;
	}
	struct __list_node_t *trav = (struct __list_node_t *)it->curr_location;
	void *data = trav->data;
	it->curr_location = (void*)(trav->next);
	return data;
}

void *__bi_nxt_iter_fn(iterator_t *it)
{
	list_t *list = (list_t *)it->structure;
	if(!it->curr_location){
		return NULL;
	}
	struct __list_node_t *trav = (struct __list_node_t *)it->curr_location;
	void *data = trav->data;
	it->curr_location = (void*)(trav->next);
	return data;
}

void *__bi_prev_iter_fn(iterator_t *it)
{
	list_t *list = (list_t *)it->structure;
	if(!it->curr_location){
		return NULL;
	}
	struct __list_node_t *trav = (struct __list_node_t *)it->curr_location;
	void *data = trav->data;
	it->curr_location = (void*)(trav->prev);
	return data;
}

void *__back_nxt_iter_fn(back_iterator_t *it)
{
	list_t *list = (list_t *)it->structure;
	if(!it->curr_location){
		return NULL;
	}
	struct __list_node_t *trav = (struct __list_node_t *)it->curr_location;
	void *data = trav->data;
	it->curr_location = (void*)(trav->prev);
	return data;	
}

iterator_t *iterator(list_t *list)
{
	if(!list){
		return NULL;
	}
	iterator_t *it = (iterator_t *)list->allocator->alloc(sizeof (iterator_t));
	it->structure = list;
	it->curr_location = list->head;
	it->next = &__bi_nxt_iter_fn;
	it->prev = &__bi_prev_iter_fn;
	return it;
}

forward_iterator_t *forward_iterator(list_t *list)
{
	if(!list){
		return NULL;
	}
	forward_iterator_t *it = (forward_iterator_t *)list->allocator->alloc(sizeof (forward_iterator_t));
	it->structure = list;
	it->curr_location = list->head;
	it->next = &__forward_nxt_iter_fn;
	return it;
}

back_iterator_t *back_iterator(list_t *list)
{
	if(!list){
		return NULL;
	}
	back_iterator_t *it = (back_iterator_t *)list->allocator->alloc(sizeof (back_iterator_t));
	it->structure = list;
	it->curr_location = list->tail;
	it->next = &__back_nxt_iter_fn;
	return it;
}

void destroy(list_t *list)
{
	if(!list){
		return;
	}
	struct __list_node_t *trav;
	for(trav = list->head; trav; trav = trav->next){
		list->allocator->dealloc(trav);
	}
	//catch-22: cant use dealloc to dealloc the allocator, use free()
	free(list->allocator);
	list->head = NULL;
	list->tail = NULL;
}

void *fold_left(list_t *list, void *(*fn)(void*, void*), void *accum)
{
	forward_iterator_t *it = forward_iterator(list); 
	void *curr;
	while((curr = it->next(it))){
		accum = fn(curr, accum);
	}
	return accum;
}

void *fold_right(list_t *list, void *(*fn)(void*, void*), void *accum)
{
	back_iterator_t *it = back_iterator(list); 
	void *curr;
	while((curr = it->next(it))){
		accum = fn(curr, accum);
	}
	return accum;
}