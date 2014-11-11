#ifndef VECTOR_H__
#define VECTOR_H__

typedef struct vector_ {
	void** data;
        int size;
	int count;
}vector;

int init(vector*);
void* add(vector*, void*);
void* delete(vector*, int);
void* get(vector*, int);
void* set(vector*, int, void*);
#endif
