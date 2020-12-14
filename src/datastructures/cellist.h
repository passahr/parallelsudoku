#ifndef _CELLIST_H
#define _CELLIST_H


typedef struct {
    int i;
    int j;
}cell_struct;


typedef struct cellist cellist;
void cell_destroy(cellist *l);
void cell_splice(cellist *l,cellist *source,unsigned int index);
void cell_join(cellist *l,cellist *source);
cellist *cell_copy(cellist *l);
cellist *cell_slice_end(cellist *l,unsigned int index);
cellist *cell_slice(cellist *l,unsigned int index,unsigned int length);
void cell_clear(cellist *l);
cell_struct cell_remove(cellist *l,unsigned int index);
cell_struct celllist_remove(cellist* l, cell_struct c);
void cell_insert(cellist *l,unsigned int index,cell_struct value);
void cell_set(cellist *l,unsigned int index,cell_struct value);
cell_struct cell_get(cellist *l,unsigned int index);
cell_struct cell_pop(cellist *l);
void cell_add(cellist *l,cell_struct item);
inline unsigned int cell_size(cellist *l);
void cell_allocate(cellist *l,unsigned int size);
void cell_print(cellist* l);
cellist* cell_create();
int cellist_equal(cellist* a, cellist* b);
/*************************************************
 * 
 * 				AGGIUNTE DA ME
 * 
 *************************************************/
//cellist* add_possibili(int* possibili, int size);
//cellist* intersection(c* a, arraylist* b, arraylist* c);
//cellist* unione(arraylist* a, arraylist* b);
//int equal(arraylist* a, arraylist* b);

int search_cell(cellist* arr, cell_struct cella);
int _binarySearch_cell(cellist* arr, int start, int end, cell_struct element);
int binarySearch_cell(cellist* arr, cell_struct element);
int a_major_than_b(cell_struct a, cell_struct b);
int equal_cell(cell_struct a, cell_struct b);
int in_cell(int i, int j, cellist* l);
cell_struct celllist_remove(cellist* l, cell_struct c);

/*************************************************
 * 
 *************************************************/

#define arraylist_iterate(l, index, item) \
	for (index = 0, item = l->body[0]; index < l->size; item = l->body[++index])
struct cellist {
	unsigned int size; // Count of items currently in list
	unsigned int capacity; // Allocated memory size, in items
	cell_struct* body; // Pointer to allocated memory for items (of size capacity * sizeof(void*))
};
#define INTERFACE 0



#endif