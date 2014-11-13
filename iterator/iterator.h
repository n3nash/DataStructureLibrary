#ifndef __ITERATOR_H_
#define __ITERATOR_H_

typedef struct __bi_iterator 
{
	void *structure;
	void *curr_location;

	void *(*next)(struct __bi_iterator *);
	void *(*prev)(struct __bi_iterator *);
} iterator_t;


typedef struct __forward_iterator
{
	void *structure;
	void *curr_location;

	void *(*next)(struct __forward_iterator *);
} forward_iterator_t;


typedef struct __back_iterator
{
	void *structure;
	void *curr_location;

	void *(*next)(struct __back_iterator *);
} back_iterator_t;

#endif