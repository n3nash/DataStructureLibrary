#include "vector.h"
#include<stdio.h>
#include<stdlib.h>

int init(vector *v) {
  
     v->data = NULL;
     v->count = 0;
     v->size = 0;
     return 1;
}

void* add(vector *v, void *data) {
     if(v==NULL || v->size == 0) {
	v->data = (void**)malloc(sizeof(void*)*1.5);
        v->size = 10;
	}
     else if(v->size == v->count) {
	v->data = realloc(v->data, sizeof(void*) * (v->size*1.5));
     }
     v->data[v->count] = data;
     v->count += 1;
     v->size *= 1.5;
     return data;
}

void* get(vector *v, int index) {
     if(v==NULL || v->size == 0 || v->count <= index){
	return NULL;
	}
     return v->data[index];
}

void* set(vector *v, int index, void* data) {
     
     if(v==NULL || v->size == 0 || v->size <= index){
        return NULL;
        }
     v->data[index] = data;
     return data;
}

void* delete(vector *v, int index) {
     if(v==NULL || v->size == 0 || v->count <= index){
        return NULL;
        }
     void *retData = v->data[index];
     v->data[index] = NULL;
     void **replaceArr = (void**)malloc(sizeof(void*)*(v->size)); // possible optimization where check if we can allocate more space now and omit one realloc step later
     int i = 0;
     while(i < v->count) {
	if(i != index) {
	replaceArr[i] = v->data[i];
	} i++; }
     free(v->data);
     v->data = replaceArr;
     v->count--;
     return retData;
}

int main() {
   vector v;
   init(&v);
   int value = 10;
   int *passVal = &value;
   int *insertData = (int*)add(&v,passVal);
   printf("Inserted data is %d", *insertData);
   printf("\n");
   int *insertedData = (int*)get(&v,0);
   printf("retrieved data is %d", *insertedData);
   printf("\n");
   if(*insertData == *insertedData) {
       printf("cool");
       printf("\n");
   }
   else {
	return -1;
   }
   value = 90;
   int *setData = (int*)set(&v,0,passVal);
   printf("New set data is %d", *setData);
   printf("\n");
   if(*setData != 90) {
        return -1;
   }
   int *deleteData = (int*)delete(&v,0);
   if(*deleteData != *setData) {
	return -1;
   }
   printf("deleted data is %d ", *deleteData);

}
