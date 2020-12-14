#ifndef _STACK_H
#define _STACK_H

/* This file was automatically generated.  Do not edit! */

typedef struct {
    
    int len;
    int** array;
}matr;

typedef struct matrstack matrstack;

matr matrstack_get(matrstack *l,unsigned int index);
matr matrstack_pop(matrstack *l);
void matrstack_add(matrstack *l,matr item);
void matrstack_allocate(matrstack *l,unsigned int size);
void print_matr(matr m);
matrstack* matrstack_create();
int** matr_create(int SIZE);



#define matrstack_iterate(l, index, item) \
	for (index = 0, item = l->body[0]; index < l->size; item = l->body[++index])
struct matrstack {
	unsigned int size; // Count of items currently in list
	unsigned int capacity; // Allocated memory size, in items
	matr* body; // Pointer to allocated memory for items (of size capacity * sizeof(void*))
};
#define INTERFACE 0

#endif