/**
 * Arraylist implementation
 * (c) 2011 @marekweb
 *
 * Uses dynamic extensible arrays.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "arraylist.h"

/*
 * Interface section used for `makeheaders`.
 */
#if INTERFACE

struct arraylist {
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
arraylist* arraylist_create()
{
	arraylist* new_list = malloc(sizeof(arraylist));
	new_list->size = 0;
	// Allocate the array
	new_list->body = malloc(sizeof(int) * ARRAYLIST_INITIAL_CAPACITY);
	assert(new_list->body);
	new_list->capacity = ARRAYLIST_INITIAL_CAPACITY;
	return new_list;
}

/**
 * Allocate sufficient array capacity for at least `size` elements.
 */
void arraylist_allocate(arraylist* l, unsigned int size)
{
	assert(size > 0);
	if (size > l->capacity) {
		unsigned int new_capacity = l->capacity;
		while (new_capacity < size) {
			new_capacity *= 2;
		}
		l->body = realloc(l->body, sizeof(int) * new_capacity);
		assert(l->body);
		l->capacity = new_capacity;
	}
}

/** 
 * Return the number of items contained in the list.
 */
extern inline unsigned int arraylist_size(arraylist*l) {
	return l->size;
}

/**
 * Add item at the end of the list.
 */
void arraylist_add(arraylist* l, int item)
{
	arraylist_allocate(l, l->size + 1);
	l->body[l->size++] = item;
}

/**
 * Pop (remove and return) an item off the end of the list.
 */
int arraylist_pop(arraylist* l)
{
	assert(l->size > 0);
	return l->body[--l->size];
}

/**
 * Return item located at index.
 */
int arraylist_get(arraylist* l, unsigned int index)
{
	assert(index < l->size);
	return l->body[index];
}


/**
 * Replace item at index with given value.
 */
void arraylist_set(arraylist* l, unsigned int index, int value)
{
	assert(index < l->size);
	l->body[index] = value;
}

/**
 * Insert item at index, shifting the following items by one spot.
 */
void arraylist_insert(arraylist* l, unsigned int index, int value)
{
	// Reallocate, if needed
	arraylist_allocate(l, l->size + 1);

	// Move data to create a spot for the new value
	arraylist_memshift(l->body + index, 1, l->size - index);
	l->body[index] = value;
	l->size++;
}

/**
 * Remove the item at index, shifting the following items back by one spot.
 */
int arraylist_remove(arraylist* l, unsigned int index)
{
	int value = l->body[index];
	arraylist_memshift(l->body + index + 1, -1, l->size - index);
	l->size--;
	return value;
}

/**
 * Clear list of all items.
 */
void arraylist_clear(arraylist* l)
{
	l->size = 0;
}

/**
 * Return a slice of the list (of given length starting at index) as a new arraylist.
 */
arraylist* arraylist_slice(arraylist* l, unsigned int index, unsigned int length)
{
	assert(index + length <= l->size);
	arraylist* new_list = arraylist_create();
	arraylist_allocate(new_list, length);
	memmove(new_list->body, l->body + index, length * sizeof(int));
	new_list->size = length;
	return new_list;
}

/**
 * Return a slice of the list (from index to the end) as a new arraylist.
 */
arraylist* arraylist_slice_end(arraylist* l, unsigned int index)
{
	return arraylist_slice(l, index, l->size - index);
}

/**
 * Return a copy of the arraylist.
 */
arraylist* arraylist_copy(arraylist* l)
{
	return arraylist_slice_end(l, 0);
}

/**
 * Append a list onto another, in-place.
 */
void arraylist_join(arraylist* l, arraylist* source)
{
	arraylist_splice(l, source, l->size);
}

/**
 * Insert a list into another at the given index, in-place.
 */
void arraylist_splice(arraylist* l, arraylist* source, unsigned int index)
{
	// Reallocate, if needed
	arraylist_allocate(l, l->size + source->size);

	// Move data to the right
	arraylist_memshift(l->body + index, source->size, l->size - index);

	// Copy the data over
	memmove(l->body + index, source->body, source->size * sizeof(int));
	l->size += source->size;
}

void arraylist_destroy(arraylist* l)
{
	free(l->body);
	free(l);
}

void arraylist_print(arraylist* l){
	if (l->size == 1){
		//printf("size: %d\n", l->size);
		printf("[%d]", l->body[0]);
	}
	else if (l->size == 2){
		//printf("size: %d\n", l->size);
		printf("[%d, %d]", l->body[0], l->body[1]);
	}
	else{
		//printf("size: %d\n", l->size);
		printf("[%d", l->body[0]);
		for (int i = 1; i < l->size - 1; i++)
			printf(", %d", l->body[i]);
		printf(", %d]", l->body[l->size - 1]);
	}
}

int binarySearch(arraylist* arr, int element){ 
	return _binarySearch(arr, 0, arr->size - 1, element);
}

int _binarySearch(arraylist* arr, int start, int end, int element){
    if (end >= start) { 
        int mid = start + (end - start) / 2; 
        if (arraylist_get(arr, mid) == element) 
            return mid; 
        if (arraylist_get(arr, mid) > element) 
            return _binarySearch(arr, start, mid - 1, element); 
        return _binarySearch(arr, mid + 1, end, element); 
    } 
    return -1; 
}

int search(arraylist* arr, int element){
	for (int i = 0; i < arr->size; i++){
		if (element == arraylist_get(arr, i))
			return i;
	}
	return -1;
}

int add_possibili(arraylist* arr, int* possibili, int size){
	/************************************
	 * INPUT: 	ARRAY STATICO CON 
	 * 			POSSIBILI VALORI
	 * OUTPUT:  LISTA CHE CONTIENE SOLO I
	 * 			POSSIBILI VALORI
	 ************************************/
	if (arr->size != 0)
		arraylist_clear(arr);
	for (int x = 0; x < size; x++){
        if (possibili[x] != 0)
            arraylist_add(arr, possibili[x]);
    }
	return 0;
}

arraylist* zerolist(){
	arraylist* r = arraylist_create();
	arraylist_add(r, 0);
	return r;
}

int intersection(arraylist* res, arraylist* a, arraylist* b, arraylist* c){
	/**************************************************
	 * 
	 * 	INPUT: 	3 INSIEMI
	 * 	OUTPUT: INTERSEZIONE TRA I 3
	 * 	OTTIENE L'ARRAY PIU CORTO E PER OGNI ELEMENTO
	 *	NE VERIFICA LA PRESENZA IN QUELLO DI LUNGHEZZA
	 *	INTERMEDIA (SE NON PRESENTE PASSA ALL'ELEMENTO
	 *  SUCCESSIVO).
	 *  IN CASO DI INISIEME VUOTO RESTITUISCE NULL
	 * 
	 **************************************************/
	if (res->size != 0)
		arraylist_clear(res);
	arraylist* vec[3] = {a, b, c};
	for (int i = 1; i < 3; i++){
		for (int j = i;  j > 0 && vec[j-1]->size > vec[j]->size; j--){
			arraylist* temp = vec[j-1];
			vec[j-1] = vec[j];
			vec[j] = temp;
		}
	}
	arraylist* ins_1 = vec[0];
	arraylist* ins_2 = vec[1];
	arraylist* ins_3 = vec[2];
	for (int j = 0; j < ins_1->size; j++){
		int element = arraylist_get(ins_1, j);
		if (binarySearch(ins_2, element) != -1){
			if (binarySearch(ins_3, element) != -1){
				//printf("aggiungo: %d\n", element);
				arraylist_add(res, element);
			}
		}	
	}
	//arraylist_print(inters);
	//printf("++++++++++++\n");
	if (res->size == 0)
		return 0;
	return 1;
}

arraylist* unione(arraylist* a, arraylist* b){
	arraylist* u = arraylist_create();
	for (int i = 0; i < a->size; i++)
		arraylist_add(u, arraylist_get(a, i));
	for (int i = 0; i < b->size; i++){
		if (search(u, arraylist_get(b, i)) == -1)
			arraylist_add(u, arraylist_get(b, i));
	}
	return u;
}

int equal(arraylist* a, arraylist* b){
	if (a->size != b->size)
		return -1;
	for (int i = 0; i < a->size; i++){
		if (arraylist_get(a, i) != arraylist_get(b, i))
			return -1;
	}
	return 0;
}