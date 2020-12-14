#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "arraylist.h"
#include "cellist.h"
#include "preemptive_set.h"

#define arraylist_memshift(s, offset, length) memmove((s) + (offset), (s), (length)* sizeof(s));
#define ARRAYLIST_INITIAL_CAPACITY 4

void preemp_allocate(prelist* l, unsigned int size){
	assert(size > 0);
	if (size > l->capacity) {
		unsigned int new_capacity = l->capacity;
		while (new_capacity < size) {
			new_capacity *= 2;
		}
		l->body = realloc(l->body, sizeof(preemptive_struct) * new_capacity);
		assert(l->body);
		l->capacity = new_capacity;
	}
}
void preemp_add(prelist *p, preemptive_struct item){
	preemp_allocate(p, p->size + 1);
	p->body[p->size++] = item;
}
preemptive_struct preemp_remove(prelist* l,unsigned int index){
    preemptive_struct value = l->body[index];
	arraylist_memshift(l->body + index + 1, -1, l->size - index);
	l->size--;
	return value;
}
prelist* preemp_create(){
	prelist* new_list = malloc(sizeof(prelist));
	new_list->size = 0;
	// Allocate the array
	new_list->body = malloc(sizeof(preemptive_struct) * ARRAYLIST_INITIAL_CAPACITY);
	assert(new_list->body);
	new_list->capacity = ARRAYLIST_INITIAL_CAPACITY;
	return new_list;
}

preemptive_struct preemp_get(prelist* l, unsigned int index){
	assert(index < l->size);
	return l->body[index];
}

void preemp_print(prelist* p){
    if (p != NULL){
    for (int i = 0; i < p->size; i++){
        preemptive_struct ps = preemp_get(p, i);
        printf("{");
        arraylist_print(ps.values);
        printf(", ");
        cell_print(ps.cells);
        printf("}\n");
    }
    }
}

void p_print(preemptive_struct ps){
    printf("{");
    arraylist_print(ps.values);
    printf(", ");
    cell_print(ps.cells);
    printf("}\n");
}

int same_row(preemptive_struct* p){
    //printf("CELLS SIZE: %d\n", p->cells->size);
    //printf("SAME ROW PREEMPTIVE_SET.c\n");
    cellist* celle = p->cells;
    int row = celle->body[0].i;
   // printf("QUI %d, PREEMPTIVE_SET.c\n", row);
    for (int i = 1; i < celle->size; i++){
        if (celle->body[i].i != row)
            return -1;
    }
    return 0;
}

int same_col(preemptive_struct* p){
    //printf("SAME COL PREEMPTIVE_SET.c\n");
    cellist* celle = p->cells;
    int col = celle->body[0].j;
    for (int i = 1; i < celle->size; i++){
        if (celle->body[i].j != col)
            return -1;
    }
    return 0;
}

