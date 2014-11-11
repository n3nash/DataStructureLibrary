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
	v->data = (void**)malloc(sizeof(void*)*1.5);
	}
     else if(v->size == v->count) {
	v->data = realloc(v->data, sizeof(void*) * (v->size*1.5));
     }
     v->data[v->count] = data;
     v->count += 1;
     v->size *= 1.5;
}

void* get(vector *v, int index) {
     if(v==null || v->size == 0 || v->count < index){
	return null;
	}
     return v->data[index];
}

void* set(vector *v, int index, void* data) {
     if(v==null || v->size == 0 || v->count < index){
        return null;
        }
     void *retData = v->data[index];
     v->data[index] = data;
}

void* delete(vector *v, int index) {
     if(v==null || v->size == 0 || v->count < index){
        return null;
        }
     v->data[index] = null;
     void **replaceArr = (void**)malloc(sizeof(void*)*(v->size)); // possible optimization where check if we can allocate more space now and omit one realloc step later
     int i = 0;
     while(i < v->size) {
	if(i != index) {
	replaceArr[i] = v->data[i];
	i++;
	}}
     free(v->data);
     v->data = replaceArr;
     v->count--;
}
