#ifndef _SUDOKU_CROOK_H
#define _SUDOKU_CROOK_H

#include <time.h>

#include "../datastructures/arraylist.h"
#include "../datastructures/preemptive_set.h"
#include "../datastructures/cellist.h"
#include "../datastructures/stack.h"
//#include "../datastructures/list.h"

typedef struct
{
    unsigned int size;     // Count of items currently in list
    unsigned int capacity; // Allocated memory size, in items
    cellist **body;        // Pointer to allocated memory for items (of size capacity * sizeof(void*))
} list_cellist;

typedef struct{
    int value;
    int i;
    int** sudoku;
    int** r;
    int** c;
    int** b;
    int*** cell;
    cellist* empty_cell;
    list_cellist* stack_cell;
    prelist* sets;
    cell_struct cella;
    clock_t t1;

}args;

/****************************************************
 * 
 *                  PROTOTIPI
 * 
 ****************************************************/

int restore_sudoku(int** sudoku, int** r, int** c, int** b, cellist* empty_cell);
int update(int upd, int** sudoku, int** r, int** c, int** b, int*** cell, cellist* empty_cell, prelist* sets);
int markup(int** sudoku, int i, int** righe, int** colonne, int** boxs);
int markup_row(int i, int** sudoku, int** righe);
int markup_col(int i, int** sudoku, int** colonne);
int markup_box(int i, int** sudoku, int** boxs);

int check_row (int i, int j, int** sudoku);
int check_col (int i, int j, int** sudoku);
int check_box (int i, int j, int** sudoku);
int check_sudo(int i, int j, int** sudoku);

int get_i_box(int i);
int get_j_box(int i);
int get_box(int i, int j);
int markup_cell(int i, int j, int update, int** sudoku, int** righe, int** colonne, int** boxs, int*** cell, cellist* empty_cell);
int step2(int update, int** sudoku, int** righe, int** colonne, int** boxs, int*** cell, cellist* empty_cell);
int find_preemptive(int** sudoku, int** r, int** c, int** b, int*** cell, cellist* empty_cell, prelist *sets);
int cross_out(int i, int** sudoku, int*** cell, cellist* empty_cell, prelist* sets);

int cross_out_row(arraylist *p, preemptive_struct ps, int** sudoku, int*** cell, cellist* empty_cell);
int cross_out_col(arraylist *p, preemptive_struct ps, int** sudoku, int*** cell, cellist* empty_cell);
int cross_out_box(arraylist *v, preemptive_struct ps, int** sudoku, int*** cell, cellist* empty_cell);

int same_box(preemptive_struct *p);
int same_box_cell(cellist *l);
int same_col_cell(cellist *l);
int same_row_cell(cellist *l);

int force(int i, int** sudoku, int** righe, int** colonne, int** boxs, int*** cell, cellist* empty_cell, list_cellist* stack_cell, prelist* sets);
int** read_input(char *filename);
void print_sudoku(int** sudoku);
void print_riga_i(int i, int** r);
void print_boxs_i(int i, int** b);
void print_col_i (int i, int** c);
void print_markup_cell(int x, int y, int*** cell);

void clear_preemptive();

int destroy_cell();
int*** init_cell(int** sudoku);

list_cellist *find_preemptive_rec_i(int k, int*** cell, cellist* empty_cell, prelist *sets);

#endif