#ifndef __LIST_H_
#define __LIST_H_

#include "../iterator/iterator.h"
struct __list_node_t
{
	void *data;
	struct __list_node_t *next;
	struct __list_node_t *prev;
};


struct __list_allocator_t
{
	void *(*alloc)(unsigned long);
	void (*dealloc)(void*);
};

typedef struct __list_t
{
	struct __list_node_t *head;
	struct __list_node_t *tail;
	struct __list_allocator_t *allocator;
	int size;
} list_t;



void init(list_t *list);
void init_with_allocator(list_t *list, void *(*allocator)(unsigned long), void (*deallocator)(void*));
void set_allocator(list_t *list, void *(*allocator)(unsigned long), void (*deallocator)(void*));
void add_front(list_t *list, void *data);
void add_back(list_t *list, void *data);
void add_at(list_t *list, void *data, int pos);
void remove_data(list_t *list, void *data, int (*cmp_fn)(void*, void*));
void remove_all(list_t *list, void *data, int (*cmp_fn)(void*, void*));
void remove_at(list_t *list, int pos);
void *get(list_t *list, int pos);
iterator_t *iterator(list_t *list);
forward_iterator_t *forward_iterator(list_t *list);
back_iterator_t *back_iterator(list_t *list);
void destroy(list_t *list);
void *fold_left(list_t *list, void *(*fn)(void*, void*), void *accum);
void *fold_right(list_t *list, void *(*fn)(void*, void*), void *accum);
#endif