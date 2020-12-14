#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "stack.h"

#define ARRAYLIST_INITIAL_CAPACITY 4


#define arraylist_memshift(s, offset, length) memmove((s) + (offset), (s), (length)* sizeof(s));


matr matrstack_get(matrstack *l,unsigned int index){
    assert(index < l->size);
	return l->body[index];
}

matr matrstack_pop(matrstack *l){
    assert(l->size > 0);
	return l->body[--l->size];
}

void matrstack_add(matrstack *l,matr item){
    matrstack_allocate(l, l->size + 1);
	l->body[l->size++] = item;
}

void matrstack_allocate(matrstack* l, unsigned int size)
{
	assert(size > 0);
	if (size > l->capacity) {
		unsigned int new_capacity = l->capacity;
		while (new_capacity < size) {
			new_capacity *= 2;
		}
		l->body = realloc(l->body, sizeof(matr) * new_capacity);
		assert(l->body);
		l->capacity = new_capacity;
	}
}

void print_matr(matr m){
	for(int i = 0; i < m.len; i++)
		printf(" ___");
	printf("\n");

	for(int i = 0; i < m.len; i++){
		for(int j = 0; j < m.len; j++){
			printf("| %d ", m.array[i][j]);
		}
		printf("|\n");
	}
}

matrstack* matrstack_create()
{
	matrstack* new_list = malloc(sizeof(matrstack));
	new_list->size = 0;
	// Allocate the array
	new_list->body = malloc(sizeof(matr) * ARRAYLIST_INITIAL_CAPACITY);
	assert(new_list->body);
	new_list->capacity = ARRAYLIST_INITIAL_CAPACITY;
	return new_list;
}

int** matr_create(int SIZE){
    int** arr = (int **) malloc(SIZE * sizeof(int*));
    for (int z = 0; z < SIZE; z++) 
        arr[z] = (int *) malloc(SIZE * sizeof(int));
    return arr;
}


