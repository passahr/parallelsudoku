#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cellist.h"

/*
 * Interface section used for `makeheaders`.
 */
#if INTERFACE

struct cellist {
	unsigned int size; // Count of items currently in list
	unsigned int capacity; // Allocated memory size, in items
	void** body; // Pointer to allocated memory for items (of size capacity * sizeof(void*))
};

/**
 * Iterates over a list, using the provided `unsigned int index` and `void* item`
 * variables as containers.
 */
#define arraylist_iterate(l, index, item) \
	for (index = 0, item = l->body[0]; index < l->size; item = l->body[++index])

#endif

// Initial capacity of the arraylist
#define ARRAYLIST_INITIAL_CAPACITY 4

/**
 * Macro to shift a section of memory by an offset, used when inserting or removing items.
 */
#define arraylist_memshift(s, offset, length) memmove((s) + (offset), (s), (length)* sizeof(s));

/**
 * Create a new, empty arraylist.
 */
cellist* cell_create()
{
	cellist* new_list = malloc(sizeof(cellist));
	new_list->size = 0;
	// Allocate the array
	new_list->body = malloc(sizeof(cell_struct) * ARRAYLIST_INITIAL_CAPACITY);
	assert(new_list->body);
	new_list->capacity = ARRAYLIST_INITIAL_CAPACITY;
	return new_list;
}

/**
 * Allocate sufficient array capacity for at least `size` elements.
 */
void cell_allocate(cellist* l, unsigned int size)
{
	assert(size > 0);
	if (size > l->capacity) {
		unsigned int new_capacity = l->capacity;
		while (new_capacity < size) {
			new_capacity *= 2;
		}
		l->body = realloc(l->body, sizeof(cell_struct) * new_capacity);
		assert(l->body);
		l->capacity = new_capacity;
	}
}

/** 
 * Return the number of items contained in the list.
 */
extern inline unsigned int cell_size(cellist* l) {
	return l->size;
}

/**
 * Add item at the end of the list.
 */
void cell_add(cellist* l, cell_struct item)
{
    //printf("ADDING: c[%d][%d]\n", item.i, item.j);
	cell_allocate(l, l->size + 1);
	l->body[l->size++] = item;
}

/**
 * Pop (remove and return) an item off the end of the list.
 */
cell_struct cell_pop(cellist* l)
{
	assert(l->size > 0);
	return l->body[--l->size];
}

/**
 * Return item located at index.
 */
cell_struct cell_get(cellist* l, unsigned int index)
{
	assert(index < l->size);
	return l->body[index];
}

/**
 * Replace item at index with given value.
 */
void cell_set(cellist* l, unsigned int index, cell_struct value)
{
	assert(index < l->size);
	l->body[index] = value;
}

/**
 * Insert item at index, shifting the following items by one spot.
 */
void cell_insert(cellist* l, unsigned int index, cell_struct value)
{
	// Reallocate, if needed
	cell_allocate(l, l->size + 1);

	// Move data to create a spot for the new value
	arraylist_memshift(l->body + index, 1, l->size - index);
	l->body[index] = value;
	l->size++;
}

/**
 * Remove the item at index, shifting the following items back by one spot.
 */
cell_struct cell_remove(cellist* l, unsigned int index)
{
	cell_struct value = l->body[index];
	arraylist_memshift(l->body + index + 1, -1, l->size - index);
	l->size--;
	return value;
}

/**
 * Clear list of all items.
 */
void cell_clear(cellist* l)
{
	l->size = 0;
}

/**
 * Return a slice of the list (of given length starting at index) as a new arraylist.
 */
cellist* cell_slice(cellist* l, unsigned int index, unsigned int length)
{
	assert(index + length <= l->size);
	cellist* new_list = cell_create();
	cell_allocate(new_list, length);
	memmove(new_list->body, l->body + index, length * sizeof(cell_struct));
	new_list->size = length;
	return new_list;
}

/**
 * Return a slice of the list (from index to the end) as a new arraylist.
 */
cellist* cell_slice_end(cellist* l, unsigned int index)
{
	return cell_slice(l, index, l->size - index);
}

/**
 * Return a copy of the cell.
 */
cellist* cell_copy(cellist* l)
{
	return cell_slice_end(l, 0);
}

/**
 * Append a list onto another, in-place.
 */
void cell_join(cellist* l, cellist* source)
{
	cell_splice(l, source, l->size);
}

/**
 * Insert a list into another at the given index, in-place.
 */
void cell_splice(cellist* l, cellist* source, unsigned int index)
{
	// Reallocate, if needed
	cell_allocate(l, l->size + source->size);

	// Move data to the right
	arraylist_memshift(l->body + index, source->size, l->size - index);

	// Copy the data over
	memmove(l->body + index, source->body, source->size * sizeof(int));
	l->size += source->size;
}

void cell_destroy(cellist* l)
{
	free(l->body);
	free(l);
}

void cell_print(cellist* l){
	if (l->size == 1){
		//printf("size: %d\n", l->size);
		printf("[(%d, %d)]", l->body[0].i, l->body[0].j);
	}
	else if (l->size == 2){
		//printf("size: %d\n", l->size);
		printf("[(%d, %d), (%d, %d)]", l->body[0].i, l->body[0].j, l->body[1].i, l->body[1].j);
	}
	else{
		//printf("size: %d\n", l->size);
		printf("[(%d, %d)", l->body[0].i, l->body[0].j);
		for (int i = 1; i < l->size - 1; i++)
			printf(", (%d, %d)", l->body[i].i, l->body[i].j);
		printf(", (%d, %d)]", l->body[l->size - 1].i, l->body[l->size - 1].j);
	}
}

int equal_cell(cell_struct a, cell_struct b){
    if (a.i == b.i && a.j == b.j)
        return 1;
    return 0;
}

int cellist_equal(cellist* a, cellist* b){
	if (a->size == b->size){
		for (int i = 0; i < a->size; i++){
			if (search_cell(b, a->body[i]) == -1)
				return -1;
		}
		return 0;
	}
	return -1;
}

int a_major_than_b(cell_struct a, cell_struct b){
    if (a.i > b.i)
        return 1;
    else if (a.i == b.i){
        if (a.j > b.j)
            return 1;
    }
    return 0;
}

int in_cell(int i, int j, cellist* l){
	cell_struct cella = {.i = i, .j = j};
	if (search_cell(l, cella) == -1)
		return -1;
	return 0;
}

int search_cell(cellist* l, cell_struct cella){
	if (l->size == 0)
		return -1;
	for (int i = 0; i < l->size; i++){
		if (equal_cell(cella, l->body[i]) == 1)
			return i;
	}
	return -1;
}


int binarySearch_cell(cellist* arr, cell_struct element){ 
	return _binarySearch_cell(arr, 0, arr->size - 1, element);
}

int _binarySearch_cell(cellist* arr, int start, int end, cell_struct element){
    if (end >= start) { 
        int mid = start + (end - start) / 2; 

        if (equal_cell(cell_get(arr, mid), element) != 0) 
            return mid; 
        if (a_major_than_b(cell_get(arr, mid), element)) 
            return _binarySearch_cell(arr, start, mid - 1, element); 
        return _binarySearch_cell(arr, mid + 1, end, element); 
    } 
    return -1; 
}

cell_struct celllist_remove(cellist* l, cell_struct c){
    int idx = binarySearch_cell(l, c);
	cell_struct r = l->body[idx];
	//printf("index: %d\n", idx);
    if (idx != -1)
        cell_remove(l, idx);
        //printf("REMOVED: c[%d][%d]\n", c1.i, c1.j);}
	return r;
}
