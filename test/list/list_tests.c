#include "../../list/list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void *add_list(void *x, void *accum)
{
	return (void*)((int)x + (int)accum);
}

void *count_2(void *x, void *accum)
{
	return ((int) x == 2) ? (void*)((int) ++accum) : accum;
}

int is_2(void *x, void *y)
{
	return !((int) x == (int) y);
}

int main(int argc, char const *argv[])
{
	list_t *list = (list_t*)malloc(sizeof (list_t));
	init(list);

	//make a node
	add_at(list, (void*)2, 0);
	assert(list->head && list->tail);
	assert((int) get(list, 0) == 2);
	//do more adding
	int i = 3;
	for(; i < 15; ++i){
		add_front(list, (void*)i);
		assert(list->head && list->tail);
		assert((int) get(list, 0) == i);
	}
	assert((int) get(list, list->size - 1) == 2);
	assert((int) fold_left(list, add_list, (void*)0) == 104);
	assert((int) fold_left(list, add_list, (void*)0) == (int)fold_right(list, add_list, (void*)0));
	add_back(list, (void*)0);
	assert(list->tail->data == (void*)0);
	assert((int) get(list, list->size - 1) == 0);
	add_front(list, (void*)2);
	add_front(list, (void*)2);
	assert((int) fold_left(list, count_2, (void*)0) == 3);
	remove_data(list, (void*)2, is_2);
	return 0;
}