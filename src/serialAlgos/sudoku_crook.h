#ifndef _SUDOKU_CROOK_H
#define _SUDOKU_CROOK_H

#include "../datastructures/arraylist.h"
#include "../datastructures/preemptive_set.h"
#include "../datastructures/cellist.h"
#include "../datastructures/stack.h"

typedef struct
{
    unsigned int size;     // Count of items currently in list
    unsigned int capacity; // Allocated memory size, in items
    cellist **body;        // Pointer to allocated memory for items (of size capacity * sizeof(void*))
} list_cellist;

int restore_sudoku();
int update(int i);
int markup(int i);
int markup_row(int i);
int markup_col(int i);
int markup_box(int i);

int check_row(int i, int j);
int check_col(int i, int j);
int check_box(int i, int j);
int check_sudo(int i, int j);

int get_i_box(int i);
int get_j_box(int i);
int get_box(int i, int j);
int markup_cell(int i, int j, int update);
int step2(int update);
int find_preemptive();
int cross_out(int i);

int cross_out_row(arraylist *p, preemptive_struct ps);
int cross_out_col(arraylist *p, preemptive_struct ps);
int cross_out_box(arraylist *v, preemptive_struct ps);

int same_box(preemptive_struct *p);
int same_box_cell(cellist *l);
int same_col_cell(cellist *l);
int same_row_cell(cellist *l);

int force(int i);
int read_input(char *filename);
void print_sudoku();
void print_riga_i(int i);
void print_boxs_i(int i);
void print_col_i(int i);
void print_markup_cell(int x, int y);

void clear_preemptive();

int destroy_cell();
int init_cell();

list_cellist *find_preemptive_rec_i(int k);

#endif