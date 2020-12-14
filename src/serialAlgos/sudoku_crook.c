#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#include "../datastructures/arraylist.h"
#include "../datastructures/preemptive_set.h"
#include "../datastructures/cellist.h"
#include "../datastructures/stack.h"

#include "./sudoku_crook.h"

#define SSIZE 16
#define MAXCHAR 256

int SIZE = 0;

/****************************************************
 * 
 *                  PROTOTIPI
 * 
 ****************************************************/

/****************************************************
 * 
 *                  VARIABILI GLOBALI
 * 
 ****************************************************/

int sudoku[SSIZE][SSIZE];
/*****************************************************
    TRICKY:                   {8,  0, 9, 0,     10, 0, 0, 12,     16, 0, 15,0,    0, 0, 0, 4},
                              {0,0, 15, 0,     0,0,5, 0,      0, 7,0, 10,         0, 0, 13,  0},
                              {2, 0, 0,0,     0, 0, 0, 4,      0,0,0, 0,          0,16,0, 14},
                              { 0, 6,0, 3,    0, 0,14,0,      0, 0, 11, 0,        9, 0, 0,  0}, 
                              
                              { 0,0,0,0,     0, 9,0, 0,         1, 0, 0,2,        0, 0, 0, 7},
                              {11,0, 0, 0,     14, 0, 0, 6,      0, 0, 0, 0,      0, 13,0,0},
                               {0, 7, 0, 16,    0, 2, 0, 0,     12,0,0, 0,        0, 0, 1, 0},
                               {0, 0, 0, 5,     8,0, 0,13,      0, 3, 0, 0,        0,0, 0, 9},
                                                                                                                
                              {12, 0,4,0,      0, 0, 7, 0,     13,0, 8, 0,      11, 0, 0, 0},
                               {0, 5, 0, 8,     0, 13, 0, 10,      0, 16, 0,4,     0, 0, 0,0},
                               {0, 0, 14, 0,     0,0, 9, 0,      6, 0,0, 11,      0,1, 0, 3},
                              {1,0, 0, 0,      3, 0,0,5,      0, 0, 0, 0,      6, 0,16, 0},
                              
                              {14,0, 7,0,      0, 0, 10,0,      0,12,0, 9,      0, 8, 2, 0},
                               {0, 16,0, 12,      0, 14, 0, 3,     0, 0, 0, 0,     13,0, 0, 11},
                               {5, 10, 0, 0,      6,0,11, 0,     0, 0, 14,0,      0, 7, 0,0},
                               {15, 0, 1, 0,     0, 0, 0, 2,      0, 0, 0, 5,      0, 0,0, 12}};

                               SOLVED ?

  
 ****************************************************/

int found = 0;
int intervallo = 0;

int righe[SSIZE][SSIZE];
int colonne[SSIZE][SSIZE];
int boxs[SSIZE][SSIZE];
int cell[SSIZE][SSIZE][SSIZE];

int size_stack;
cellist **stack_cell;
cellist *empty_cell;
prelist *sets;

/****************************************************
 * 
 *                  DEFINIZIONE FUNZIONI
 * 
 ****************************************************/

void print_riga_i(int i)
{
    printf("[%d", righe[i][0]);
    for (int j = 1; j < SIZE - 1; j++)
        printf(", %d", righe[i][j]);
    printf(", %d]", righe[i][SIZE - 1]);
    printf("\n");
}

void print_col_i(int i)
{
    printf("[%d", colonne[i][0]);
    for (int j = 1; j < SIZE - 1; j++)
        printf(", %d", colonne[i][j]);
    printf(", %d]", colonne[i][SIZE - 1]);
    printf("\n");
}

void print_boxs_i(int i)
{
    printf("[%d", boxs[i][0]);
    for (int j = 1; j < SIZE - 1; j++)
        printf(", %d", boxs[i][j]);
    printf(", %d]", boxs[i][SIZE - 1]);
    printf("\n");
}

void print_markup_cell(int x, int y)
{
    printf("[%d", cell[x][y][0]);
    for (int i = 1; i < SIZE - 1; i++)
        printf(", %d", cell[x][y][i]);
    printf(", %d]", cell[x][y][SIZE - 1]);
    printf("\n");
}

int markup(int i)
{
    /********************************************
     * INPUT:       Indice i
     * OUTPUT:      Intero
     * DESCRIZIONE: Chiama le funzioni markup_row, markup_col e markup_box le quali
     *              calcolano correttamente l'insieme dei possibili valori inseribili
     *              rispettivamente in riga(i), colonna(i), box(i).
     * 
     ********************************************/
    markup_row(i);
    markup_col(i);
    markup_box(i);
    return 0;
}

int markup_row(int i)
{
    for (int k = 0; k < SIZE; k++)
        righe[i][k] = k + 1;
    for (int j = 0; j < SIZE; j++)
    {
        if (sudoku[i][j] != 0)
            righe[i][sudoku[i][j] - 1] = 0;
    }
    return 0;
}

int markup_col(int i)
{
    for (int k = 0; k < SIZE; k++)
        colonne[i][k] = k + 1;
    for (int j = 0; j < SIZE; j++)
    {
        if (sudoku[j][i] != 0)
            colonne[i][sudoku[j][i] - 1] = 0;
    }
    return 0;
}

int markup_box(int i)
{
    for (int k = 0; k < SIZE; k++)
        boxs[i][k] = k + 1;
    int x = get_i_box(i);
    int y = get_j_box(i);
    for (int row = x; row < x + sqrt(SIZE); row++)
    {
        for (int column = y; column < y + sqrt(SIZE); column++)
        {
            if (sudoku[row][column] != 0)
            {
                boxs[i][sudoku[row][column] - 1] = 0;
            }
        }
    }
    return 0;
}

int inters_static(int riga, int colonna, int box)
{
    int counter = 0; //Conta elementi in intersezione
    for (int i = 0; i < SIZE; i++)
    {
        if (righe[riga][i] != 0 && colonne[colonna][i] != 0 && boxs[box][i] != 0)
        {
            cell[riga][colonna][i] = righe[riga][i];
            counter++;
        }
        else
            cell[riga][colonna][i] = 0;
    }
    return counter;
}

int get_value_celle(int riga, int colonna)
{
    for (int i = 0; i < SIZE; i++)
        if (cell[riga][colonna][i] != 0)
            return i;
    return -1;
}

int get_size_inters(int riga, int colonna)
{
    int counter = 0;
    for (int i = 0; i < SIZE; i++)
    {
        if (cell[riga][colonna][i] != 0)
            counter++;
    }
    return counter;
}

int markup_cell(int x, int y, int update)
{
    /**************************
     * INPUT:       Coordinate x ed y della tabella
     * OUTPUT:      Un intero
     * DESCRIZIONE: Effettua l'intersezione tra i markup della riga[x], colonna[y] e box[get_box(x, y)]
     *              Se l'interesione è vuota ritorna, altrimenti controlla se l'intersezione è formato da 
     *              un elemento, in caso affermativo allora lo inserisce nel sudoku, quindi rimuove la cella
     *              dalla liste delle cella vuote, e aggiorna il markup richiamando la stessa funzione che 
     *              in modo tale da scartare ricorsivamente i valori, passo dopo passo.
     *              Nel caso in cui non trova nessun singleton allora ritorna, se la cella non è presente nella
     *              lista di celle vuote allora la aggiunge
     **************************/
    int inters = inters_static(x, y, get_box(x, y));
    if (inters == 0)
        return 2;
    if (get_size_inters(x, y) == 1)
    {
        int idx = get_value_celle(x, y);
        sudoku[x][y] = cell[x][y][idx];
        if (check_sudo(x, y) == -1)
        {
            sudoku[x][y] = 0;
            return -1;
        }
        cell[x][y][idx] = 0;
        cell_struct cella = {.i = x, .j = y};
        celllist_remove(empty_cell, cella);

        for (int i = 0; i < SIZE; i++)
            markup(i);
        int res = step2(0);
        if (res != 0)
            return res;
        return 1;
    }
    cell_struct cella = {.i = x, .j = y};
    int idx = binarySearch_cell(empty_cell, cella);
    if (idx == -1)
        cell_add(empty_cell, cella);
    return 0;
}

int get_box(int i, int j)
{
    /**************************************
     *  DATE LE COORDINATE DELLA CELLA
     *  NE OTTIENE IL BOX
     *  INPUT: RIGA, COLONNA
     *  OUTPUT: INDICE DI BOX
     **************************************/
    int x = get_i_box(i);
    int y = get_i_box(j);
    int count = 0;
    for (int xx = 0; xx < SIZE; xx += sqrt(SIZE))
    {
        for (int yy = 0; yy < SIZE; yy += sqrt(SIZE))
        {
            if (y == yy && x == xx)
                return count;
            count += 1;
        }
    }
    return count;
}

int get_j_box(int i)
{
    /**************************************
     *  OTTIENE LA COLONNA DA CUI INIZIA
     *  LA PRIMA CELLA DEL BOX
     *  INPUT: NUMERO DEL BOX
     *  OUTPUT: INDICE DI COLONNA
     **************************************/
    int step = sqrt(SIZE);
    return (i % step) * step;
}

int get_i_box(int i)
{
    /**********************************
     *  OTTIENE LA RIGA DA CUI INIZIA
     *  LA PRIMA CELLA DEL BOX
     *  INPUT: NUMERO DEL BOX
     *  OUTPUT: INDICE DI RIGA
     **********************************/
    int step = sqrt(SIZE);
    for (int x = 0; x < sqrt(SIZE); x++)
    {
        if (i < step)
            return step - intervallo;
        step += intervallo;
    }
    return step - intervallo;
}

void clear_preemptive()
{
    /********************************************
     * DESCRIZIONE: Permette di liberare la memoria occupata dai preemptive set
     ********************************************/
    for (int i = 0; i < sets->size; i++)
    {
        preemptive_struct p = preemp_remove(sets, i);
        arraylist_destroy(p.values);
        cell_destroy(p.cells);
    }
}

int check_row(int i, int j)
{
    /********************************************
     * DESCRIZIONE: Coontrolla se esiste una cella sulla stessa riga 
     *              della cella i, j che ha lo stesso valore.
     ********************************************/
    int cella = sudoku[i][j];
    for (int x = 0; x < SIZE; x++)
    { // CHECK ROW
        if (x != i)
        {
            if (sudoku[x][j] == cella)
                return -1;
        }
    }
    return 0;
}

int check_col(int i, int j)
{
    /********************************************
     * DESCRIZIONE: Coontrolla se esiste una cella sulla stessa colonna 
     *              della cella i, j che ha lo stesso valore.
     ********************************************/
    int cella = sudoku[i][j];
    for (int x = 0; x < SIZE; x++)
    { // CHECK ROW
        if (x != j)
        {
            if (sudoku[i][x] == cella)
                return -1;
        }
    }
    return 0;
}

int check_box(int i, int j)
{
    /********************************************
     * DESCRIZIONE: Coontrolla se esiste una cella nello stesso box 
     *              della cella i, j che ha lo stesso valore.
     ********************************************/
    int cella = sudoku[i][j];
    int num_box = get_box(i, j);
    int x = get_i_box(num_box);
    int y = get_j_box(num_box);
    for (int row = x; row < x + sqrt(SIZE); row++)
    {
        for (int column = y; column < y + sqrt(SIZE); column++)
        {
            if (row != i && column != j)
            {
                if (sudoku[row][column] == cella)
                    return -1;
            }
        }
    }
    return 0;
}

int check_sudo(int i, int j)
{
    /********************************************
     * DESCRIZIONE: Coontrolla se esiste una cella sulla stessa riga, colonna, box 
     *              della cella i, j 
     ********************************************/
    if (check_row(i, j) == -1 || check_col(i, j) == -1 || check_col(i, j) == -1)
        return -1;
    return 0;
}

int check()
{
    /********************
     * DESCRIZIONE: Verifica che il sudoku sia completo, se trova almeno una cella
     *              vuota allora ritorna -1
     ********************/
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (sudoku[i][j] == 0)
                return -1;
        }
    }
    return 0;
}

int step2(int update)
{
    /********************************************
     * DESCRIZIONE: Chiama la funzione che effettua il markup di ogni cella
     *              in base al ritorno della funzione ritorna lo stesso valore
     ********************************************/
    //printf("STEP PRIMA\n");
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (sudoku[i][j] == 0)
            {
                int res = markup_cell(i, j, update);
                if (res == -1)
                {
                    //printf("SUDOKU VIOLATO\n");
                    return -1;
                }
                if (res == 1)
                {
                    //printf("AGGIUNTI TUTTI I SINGLETON\n");
                    return 1;
                }
                if (res == 2)
                {
                    //printf("INTERSEZIONE VUOTA");
                    return 2;
                }
            }
        }
    }
    return 0;
}

int update(int upd)
{
    /********************************************
     * DESCRIZIONE: Chiama la funzione che effettua il markup delle righe colonne e box
     *              controlla se l'insieme dei preemptive set è vuoto, in caso di esito positivo lo libera.
     *              Successivamente chiama la funzione che effettua il markup delle celle, se la funzione torna
     *              1 allora ed il sudoku non è completo allora ho aggiunto tutti i singleton
     *              Dunque cerco i preemptive sets
     ********************************************/
    for (int i = 0; i < SIZE; i++)
        markup(i);
    //printf("MARKUP FATTO\n");
    if (sets->size != 0)
    {

        free(sets);
        sets = preemp_create();
    }
    int res = step2(upd);
    if (res == -1)
    {
        //printf("SUDOKU VIOLATO\n");
        return -1;
    }
    if ((res == 1 || res == 0) && check() != -1)
    {
        //printf("AGGIUNTI TUTTI I SINGLETON  \n");
        if (check() == -1)
            find_preemptive();
        else
            print_sudoku();
        return 1;
    }
    return res;
}

int find_preemptive()
{
    /********************************************
     * DESCRIZIONE: Chiama la funzione che trova i preemptive set
     *              Effettua il crossout dei valori presenti nel preemptive set 
     * 
     * ESEMPIO:     preemptive_set: { [2, 9], [(1,5), (1,9)]};
     *              rimuove dal markup delle celle che si trovano sulla stessa 
     *              colonna in questo caso sulla colonna 1 i valori 2 e 9.
     *              Successivamente libera memoria e richiama la funzione che aggiorna
     *              il markup e verifica l'esistenza dei singleton
     ********************************************/
    list_cellist *l = find_preemptive_rec_i(4);
    if (sets->size == 0)
        return -1;
    for (int i = 0; i < sets->size; i++)
        cross_out(i);
    for (int i = 0; i < l->size; i++)
    {
        free(l->body[i]->body); //LIBERO GLI SLOT RIMANENTI
        free(l->body[i]);
    }
    free(l); //LIBERO LO SPAZIO ALLOCATO PER LA LISTA DI LISTE DI CELLE
    int res = step2(0);
    return res;
}

list_cellist *list_cell_create()
{
    list_cellist *new_list = malloc(sizeof(list_cellist));
    new_list->size = 0;
    // Allocate the array
    new_list->body = malloc(sizeof(cell_struct) * 4);
    assert(new_list->body);
    new_list->capacity = 4;
    return new_list;
}

void list_cell_allocate(list_cellist *l, unsigned int size)
{
    assert(size > 0);
    if (size > l->capacity)
    {
        unsigned int new_capacity = l->capacity;
        while (new_capacity < size)
        {
            new_capacity *= 2;
        }
        l->body = realloc(l->body, sizeof(cellist) * new_capacity);
        assert(l->body);
        l->capacity = new_capacity;
    }
}

void list_cell_add(list_cellist *l, cellist *item)
{

    list_cell_allocate(l, l->size + 1);
    l->body[l->size++] = item;
}

arraylist *unione_static(int a[SIZE], int b[SIZE])
{
    arraylist *u = arraylist_create();
    for (int i = 0; i < SIZE; i++)
    {
        if (a[i] != 0)
            arraylist_add(u, a[i]);
    }
    for (int i = 0; i < SIZE; i++)
    {
        if (b[i] != 0 && search(u, b[i]) == -1)
            arraylist_add(u, b[i]);
    }
    return u;
}

arraylist *unione_dynamic_static(arraylist *u, int b[SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        if (b[i] != 0 && search(u, b[i]) == -1)
            arraylist_add(u, b[i]);
    }
    return u;
}

list_cellist *find_preemptive_rec_i(int k)
{
    /********************************************
     * 
     * INPUT:       Parametro k, serve per generare le permutazioni di k elementi
     * OUTPUT:      Ritorna una lista di liste di celle, ogni lista di celle è una permutazione
     * DESCRIZIONE: Se k == 2 allore è il caso base e allora calcola le permutazioni di 2 elementi, nel frattempo controlla
     *              se l'unione tra i markup delle celle è uguale a k allora è stato trovato un preemptive set.
     *              Per ogni cella nella lista di celle vuote viene permutata con le altre e dunque vengono inserite in una lista
     *              di celle chiamata 'slot', che viene passata alle funzioni same_row_cell, same_col_cell, same_box_cell
     *              le quali ci permettono di controllare se sono sulla stessa riga, colonna o box
     *              Nel caso in cui non fosse così non dobbiamo aggiungere la lista di celle nell'insieme dei preemptive set,
     *              altrimenti effettua unione.
     *              
     * 
     ********************************************/
    if (k == 2)
    {
        // TROVO PERMUTAZIONI DI 2 ELEMENTI
        list_cellist *lista_di_liste_di_celle = list_cell_create();
        for (int i = 0; i < empty_cell->size - 1; i++)
        {
            for (int j = i + 1; j < empty_cell->size; j++)
            {
                cellist *slot = cell_create();
                cell_add(slot, cell_get(empty_cell, i));
                cell_add(slot, cell_get(empty_cell, j));
                if (same_row_cell(slot) == -1 && same_col_cell(slot) == -1 && same_box_cell(slot) == -1)
                {
                    cell_destroy(slot);
                    continue;
                }
                arraylist *unione_liste = arraylist_create();
                unione_liste = unione_static(cell[cell_get(slot, 0).i][cell_get(slot, 0).j], cell[cell_get(slot, 1).i][cell_get(slot, 1).j]);
                //arraylist_print(unione_liste);
                if (unione_liste->size == k)
                {
                    //printf("PREEMPTIVE TROVATO\n");
                    preemptive_struct p = {.values = unione_liste, .cells = cell_copy(slot)};
                    preemp_add(sets, p);
                }
                else
                    arraylist_destroy(unione_liste);
                list_cell_add(lista_di_liste_di_celle, slot);
            }
        }
        return lista_di_liste_di_celle;
    }
    list_cellist *nuova_lista = list_cell_create();
    list_cellist *k_meno_uno_celle = find_preemptive_rec_i(k - 1);

    for (int i = 0; i < empty_cell->size; i++)
    {
        cell_struct a = cell_get(empty_cell, i);
        for (int j = 0; j < k_meno_uno_celle->size; j++)
        {
            if (search_cell(k_meno_uno_celle->body[j], a) == -1)
            {
                cellist *slot = cell_create();
                cell_add(slot, a);
                for (int m = 0; m < k_meno_uno_celle->body[j]->size; m++)
                    cell_add(slot, k_meno_uno_celle->body[j]->body[m]);
                if (a_major_than_b(a, slot->body[1]))
                {
                    cell_destroy(slot);
                    continue;
                }
                if (same_row_cell(slot) == -1 && same_col_cell(slot) == -1 && same_box_cell(slot) == -1)
                {
                    cell_destroy(slot);
                    continue;
                }
                arraylist *unione_liste = arraylist_create();
                unione_liste = unione_static(cell[a.i][a.j], cell[cell_get(slot, 1).i][cell_get(slot, 1).j]);
                for (int m = 1; m < slot->size - 1; m++)
                {
                    unione_liste = unione_dynamic_static(unione_liste, cell[cell_get(slot, m + 1).i][cell_get(slot, m + 1).j]);
                }
                if (unione_liste->size == k)
                {
                    //printf("PREEMPTIVE TROVATO\n");
                    preemptive_struct p = {.values = unione_liste, .cells = cell_copy(slot)};
                    preemp_add(sets, p);
                }
                else
                {
                    arraylist_destroy(unione_liste);
                }
                list_cell_add(nuova_lista, slot);
            }
        }
    }
    /**************
     *  PROBLEMA, GLI SLOT CHE NON SONO IN PREEMPTIVE_SET NON VENGONO ELIMINATI
     *  PENSO CHE QUESTO CAUSI UN USO ECCESSIVO DELLA MEMORIA
     *************/
    for (int i = 0; i < k_meno_uno_celle->size; i++)
    {
        free(k_meno_uno_celle->body[i]->body); //LIBERO GLI SLOT
        free(k_meno_uno_celle->body[i]);
    }
    free(k_meno_uno_celle); //GIUSTIFICATO DAL FATTO CHE ADESSO METTO LA COPIA DI SLOT IN .CELLS ALTRIMENTI AVREI LIBERATO ANCHE QUELLE LISTE SE AVESSI SOLO MESSO IL PUNTATORE

    return nuova_lista;
}

int same_box_cell(cellist *l)
{
    /********************************************
     * INPUT:       Lista di celle
     * OUTPUT:      -1 se non sono nello stesso box,
     *              altrimenti 0
     * DESCRIZIONE: Verifica che le celle presenti nella lista
     *              siano nello stesso box
     ********************************************/
    int num = get_box(cell_get(l, 0).i, cell_get(l, 0).j);
    for (int i = 1; i < l->size; i++)
    {
        if (get_box(cell_get(l, i).i, cell_get(l, i).j) != num)
            return -1;
    }
    return 0;
}

int same_row_cell(cellist *l)
{
    /********************************************
     * INPUT:       Lista di celle
     * OUTPUT:      -1 se non sono nello stesso box,
     *              altrimenti 0
     * DESCRIZIONE: Verifica che le celle presenti nella lista
     *              siano nello stessa riga
     ********************************************/
    int row = l->body[0].i;
    for (int i = 1; i < l->size; i++)
    {
        if (l->body[i].i != row)
            return -1;
    }
    return 0;
}

int same_col_cell(cellist *l)
{
    /********************************************
     * INPUT:       Lista di celle
     * OUTPUT:      -1 se non sono nello stesso box,
     *              altrimenti 0
     * DESCRIZIONE: Verifica che le celle presenti nella lista
     *              siano nello stessa colonna
     ********************************************/
    int col = l->body[0].j;
    for (int i = 1; i < l->size; i++)
    {
        if (l->body[i].j != col)
            return -1;
    }
    return 0;
}

int same_box(preemptive_struct *p)
{
    cellist *c = p->cells;
    int num = get_box(cell_get(c, 0).i, cell_get(c, 0).j);
    for (int i = 1; i < c->size; i++)
    {
        if (get_box(cell_get(c, i).i, cell_get(c, i).j) != num)
            return -1;
    }
    return 0;
}

int cross_out(int i)
{
    /**********************************************************
     * IMPORTANTE: Devo capire come devo eliminare i valori nelle altre celle
     * ad esempio:
     * 
     *          {[1, 3, 4], [(1, 3), (1, 4), (1, 5)]} | Questi sono tutti e tre
     *          {[1, 9, 2], [(0, 1), (0, 7), (0, 8)]} | sulla stessa riga
     *          
     *          {[2, 9, 1], [(0, 7), (0, 8), (1, 2)]} | Questi no
     * 
     * 
     *          
     **********************************************************/
    preemptive_struct p = preemp_get(sets, i);
    if (same_row(&p) == 0)
        cross_out_row(p.values, p);
    else if (same_col(&p) == 0)
        cross_out_col(p.values, p);
    //else if (same_box(&p) == 0)
    //cross_out_box(p.values, p);
    return 0;
}

int cross_out_col(arraylist *v, preemptive_struct ps)
{
    int col = ps.cells->body[0].j;
    for (int i = 0; i < SIZE; i++)
    {
        if (sudoku[i][col] == 0 && in_cell(i, col, ps.cells) == -1)
        {
            for (int k = 0; k < ps.values->size; k++)
            {
                if (cell[i][col][ps.values->body[k] - 1] != 0)
                    cell[i][col][ps.values->body[k] - 1] = 0;
                if (get_size_inters(i, col) == 1)
                {
                    //printf("SE IL MARKUP DELLA CELLA E' UN SINGLETON\n");
                    int idx = get_value_celle(i, col);
                    sudoku[i][col] = cell[i][col][idx];
                    cell_struct cella = {.i = i, .j = col};
                    celllist_remove(empty_cell, cella);
                }
            }
        }
    }
    return 0;
}

int cross_out_box(arraylist *v, preemptive_struct ps)
{
    int num_box = get_box(ps.cells->body[0].i, ps.cells->body[0].j);
    int i = get_i_box(num_box);
    int j = get_i_box(num_box);
    for (int x = i; x < i + sqrt(SIZE); x++)
    {
        for (int y = j; y < j + sqrt(SIZE); y++)
        {
            if (sudoku[x][y] == 0 && in_cell(x, y, ps.cells) == -1)
            {
                for (int k = 0; k < ps.values->size; k++)
                {
                    if (cell[x][y][ps.values->body[k] - 1] != 0)
                        cell[x][y][ps.values->body[k] - 1] = 0;
                }
                if (get_size_inters(x, y) == 1)
                {
                    int idx = get_value_celle(x, y);
                    sudoku[x][y] = cell[x][y][idx];
                    cell_struct cella = {.i = x, .j = y};
                    celllist_remove(empty_cell, cella);
                }
            }
        }
    }
    return 0;
}

int cross_out_row(arraylist *v, preemptive_struct ps)
{
    /****************************************************************************************
     * INPUT:           UNA LISTA DI VALORI ED UNA CELLA
     * OUTPUT:          NESSUNO
     * DESCRIZIONE:     ELIMINA GLI ELEMENTI PRESENTI NELLA LISTA IN INPUT DAL MARKUP DELLE
     *                  CELLE PRESENTI SULLA STESSA RIGA.
     *                  ITERA SULLE CELLE VUOTE RIMANENTI empty_cell PER OGNUNA DI QUESTE
     *                  CONTROLLA SE SI TROVA SULLA STESSA RIGA DELLA CELLA PRESENTE NEL PREEMPTIVE
     *                  SET, INOLTRE CONTROLLA CHE NON SIA LA CELLA STESSA.
     *                  UNA VOLTA VERIFICATO QUESTO OGNI VALORE PRESENTE NELLA LISTA DATA IN INPUT
     *                  VIENE ELIMINATO (SE PRESENTE) NEL MARKUP DELLA CELLA CHE SI TROVA SULLA 
     *                  STESSA RIGA DI QUELLA PASSATA IN INPUT
     *****************************************************************************************/
    int row = ps.cells->body[0].i;
    for (int j = 0; j < SIZE; j++)
    {
        if (sudoku[row][j] == 0 && in_cell(row, j, ps.cells) == -1)
        {
            //printf("i, j: (%d, %d)\n", row, j);
            for (int k = 0; k < ps.values->size; k++)
            {
                if (cell[row][j][ps.values->body[k] - 1] != 0)
                    cell[row][j][ps.values->body[k] - 1] = 0;
                if (get_size_inters(row, j) == 1)
                {
                    //printf("SE IL MARKUP DELLA CELLA E' UN SINGLETON\n");
                    int idx = get_value_celle(row, j);
                    sudoku[row][j] = cell[row][j][idx];
                    cell_struct cella = {.i = row, .j = j};
                    celllist_remove(empty_cell, cella);
                }
            }
        }
    }
    return 0;
}

int stack_add_list(cellist *l)
{
    cellist *nuovlista = cell_copy(l);
    stack_cell = realloc(stack_cell, (size_stack + 1) * sizeof(cellist *));
    stack_cell[size_stack] = nuovlista;
    size_stack += 1;
    return 0;
}

cellist *stack_pop_list()
{
    assert(size_stack > 0);
    cellist *l = stack_cell[size_stack - 1];
    size_stack -= 1;
    return l;
}

cell_struct cell_get_min(cellist *l)
{
    //OTTIENE LA CELLA CON IL MINOR NUMERO POSSIBILE DI VALORI NEL MARKUP
    cell_struct minima = l->body[0];
    for (int i = 1; i < l->size; i++)
    {
        cell_struct altra = l->body[i];
        if (get_size_inters(minima.i, minima.j) < get_size_inters(altra.i, altra.j))
            minima = altra;
    }
    return minima;
}

int restore_sudoku()
{
    /********************************************
     * DESCRIZIONE: Ripristina il sudoku dopo che l'algoritmo si accorge di aver trovato una cofigurazione sbagliata
     *              In particolare per ogni cella della liste delle celle vuote ripristina il valore 0
     *              Successivamente chiama di nuovo il markup delle righe, colonne e box
     ********************************************/
    for (int i = 0; i < empty_cell->size; i++)
    {
        cell_struct c = empty_cell->body[i];
        //arraylist_print(cell[c.i][c.j]);
        sudoku[c.i][c.j] = 0;
    }
    for (int i = 0; i < SIZE; i++)
        markup(i);
    return 0;
}

int force(int k)
{
    /********************************************
     * FORZA RICORSIVAMENTE UN NUMERO NEL MARKUP DELLA PRIMA CELLA DISPONIBILE
     * TROVA PIU' SOLUZIONI SE ESISTONO
     * 
     * INPUT:       Parametro k, lo uso per controllare la profondità della ricorsione
     *              non è utile al fine dell'algoritmo
     * OUTPUT:      Intero, se è uguale ad 1 allora ho trovato la soluzione
     * DESCRIZIONE: Prende la cella che ha il minor numero di elementi possibili (min(|MARKUP|))
     *              e per ogni valore nel markup lo prova ad inserire nella cella,
     *              se arriva ad una soluzione sbagliata ripristina il sudoku e tenta con il prossimo valore
     *              altrimenti se ha aggiunto tutti i singleton allora deve forzare di nuovo, quindi
     *              richiama se stessa e prenderà di nuovo la cella che ha il minor numero di elementi nel markup.
     * 
     ********************************************/
    if (empty_cell->size == 0)
    {
        print_sudoku();
        return 1;
    }
    cell_struct minima = cell_get_min(empty_cell);
    int i = minima.i;
    int j = minima.j;
    for (int x = 0; x < SIZE; x++)
    {
        if (cell[minima.i][minima.j][x] == 0)
            continue;
        int value = cell[minima.i][minima.j][x];
        sudoku[i][j] = value;
        if (check_sudo(i, j) == -1)
        {
            sudoku[i][j] = 0;
            continue;
        }
        sudoku[i][j] = 0;
        stack_add_list(empty_cell);
        cell_destroy(empty_cell);
        empty_cell = cell_create();
        sudoku[i][j] = value;
        celllist_remove(empty_cell, minima);
        int res = update(1); // TENTO DI TROVARE UNA SOLUZIONE
        if (res == -1 || res == 2)
        { // ARRIVO AD UNA SOLUZIONE SBAGLIATA, VIENE VIOLATO IL SUDOKU
            cellist *l = stack_pop_list();
            cell_destroy(empty_cell);
            empty_cell = l;
            clear_preemptive();
            restore_sudoku();
            //CHIAMO UPDATE PER FAR IL MARKUP DEL SUDOKU, TECNICAMENTE E' QUELLO PRECEDENTE, MA PER SEMPLICITA LO RICALCOLO OGNI VOLTA
        }
        else
        { //LA SOLUZIONE è CORRETTA, OPPURE DEVO FORZARE ANCORA PERCHE SONO ARRIVATO AD UN PUNTO DI STALLO
            if (empty_cell->size == 0)
                return 1;
            else
            { // ALTRIMENTI DEVO FORZARE ANCORA QUINDI CHIAMO LA FUNZIONE FORCE
                int res = force(k + 1);
                if (res == 1)
                    return 1;
                cellist *l = stack_pop_list();
                cell_destroy(empty_cell);
                empty_cell = l;
                restore_sudoku();
                clear_preemptive();
            }
        }
    }

    return 0;
}

int stacklist_create()
{
    stack_cell = (cellist **)malloc(sizeof(cellist *));
    size_stack += 1;
    return 0;
}

int init_cell()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (sudoku[i][j] == 0)
            {
                for (int k = 0; k < SIZE; k++)
                    cell[i][j][k] = 0;
            }
        }
    }
    return 0;
}

int markup_init()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            righe[i][j] = 0;
            colonne[i][j] = 0;
            boxs[i][j] = 0;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    time_t t;
    srand((unsigned)time(&t));
    init_cell();   //INIZIALIZZO L'ARRAY DEL MARKUP DELLE CELLE
    markup_init(); //INIZIALIZZO L'ARRAY DEI MARKUP DELLE RIGHE, COLONNE, BOXS
    if (read_input(argv[1]) == 1)
        return 0;
    clock_t t1, t2;
    t1 = clock();
    intervallo = sqrt(SIZE);
    //stack = matrstack_create();
    stacklist_create();
    empty_cell = cell_create();
    sets = preemp_create();
    printf("SUDOKU IN INPUT\n");
    print_sudoku();
    printf("\nSOLUZIONE\\I\n");
    printf("----------------------\n");
    update(0);
    if (check() == -1)
        find_preemptive(); //POTREI RIMUOVERLA
    for (int i = 0; i < SIZE; i++)
        markup(i); //RICREO IL MARKUP
    if (check() == -1)
        force(1);
    t2 = clock();
    printf("COLPI DI CLOCK %lu\n", t2 - t1);
    return 0;
}

void print_sudoku()
{
    for (int i = 0; i < SIZE; i++)
        printf(" ___");
    printf("\n");

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (SIZE > 9)
            {
                if (sudoku[i][j] > 9)
                    printf("| %d", sudoku[i][j]);
                else
                    printf("| %d ", sudoku[i][j]);
            }
            else
                printf("| %d ", sudoku[i][j]);
        }
        printf("|\n");
    }
}

int read_input(char *filename)
{
    // IL FILE DEVE AVERE DOPO L'ULTIMA CELLA UN \n
    FILE *fp;
    char line[MAXCHAR];
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("ERRORE IN APERTURA FILE %s\n", filename);
        return 1;
    }
    int i = 0;
    int j = 0;
    int lenchar = 0;
    char num[3] = "00\0";
    fgets(line, MAXCHAR, fp);
    if (line[1] != '\n')
        SIZE = 16;
    else
        SIZE = 9;
    while (fgets(line, MAXCHAR, fp))
    {
        for (int x = 0; x < MAXCHAR; x++)
        {
            if (line[x] == '\n' && x == 0)
                break;
            if (line[x] == '\n' && x != 0)
            {
                num[lenchar] = '\0';
                int value = atoi(num);
                sudoku[i][j] = value;
                lenchar = 0;
                i++;
                j = 0;
                break;
            }
            if (line[x] == '-' || line[x] == ' ')
            {
                num[lenchar] = '\0';
                int value = atoi(num);
                sudoku[i][j] = value;
                lenchar = 0;
                j++;
                continue;
            }
            num[lenchar] = line[x];
            lenchar++;
        }
    }
    return 0;
}