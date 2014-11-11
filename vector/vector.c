#include "vector.h"
#include<stdio.h>

int init(vector *v) {
     if(v!=null) {
	return -1;
	}
     v->data = null;
     v->count = 0;
     v->size = 0;
     return 1;
}

void* add(vector *v, void *data) {
     if(v==null || v->size == 0) {
	v->data = (void**)malloc(sizeof(void*)*2);
	}
     else if(v->size == v->count) {
	v->data = realloc(v->data, sizeof(void*) * (v->size*2));
     }
     v->data[v->count] = data;
     v->count += 1;
}
