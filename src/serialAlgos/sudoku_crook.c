#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	int i;
	int j;
} cella;

typedef struct {
	int* values;
	int* cella;
}preemptive_set;

typedef struct {
	int id; //Identificativo, dato dalla somma degli elementi della lista
	int len; //lunghezza della lista
	int* array;
	cella* celle;
	int size_celle;
} set;



void print_sudoku();
void print_preemptive(int idx);
void print_set(set* insieme);
void print_vec(int* array, int len);
void cancella(int num, int* array);
void check_col(int i, int j, int* array);
void check_row(int i, int j, int* array);
void check_box(int i, int j, int* array);
void alloca_markup(int i, int j, int* array);
void to_consider(int i, int j);
void is_singleton(int i, int j);
void find_preemptive_set(int m, int start);
void add_preemptive(cella* a, cella* b);
int binarySearch(int* arr, int start, int end, int element);
int unione(cella* a,  cella* b, int m);
int get_index(int value);
int crook();
int markup(int i , int j);

void add_new_set(int* arr, int size, int id, cella* a, cella* b);
int inPeemptiveSet(int* arr, int size, int sum);

/*

	AGGIORNAMENTO AL 3 DICEMBRE ALLE 2:30

		Sto implementando la funzionalità che permette di trovare i 
		preemptive set, sto dunque cercando per ogni cella da considerare
		tutte le possibili combinazioni in cui posso formare questi preemptive
		set, parto da 2 e una volta analizzate tutte le combinazioni incremento
		il numero di possibili valori che formano il preemptive set.
		Metodo brute force per individuare questi insiemi, non ne ho trovato un
		altro per il momento, vorrei cercare di ottimizzare la questione,
		altrimenti è inutile cercare di parallelizzare l'algoritmo.

*/

/*
	AGGIORNAMENTO AL 3 DICEMBRE ALLE 19:38

	Ho trovato il modo per cercare i preemptive set, uno di questi è
	PREEMPTIVE SET: [4, 5, 7, 8],

	Ho creato un array di set chiamato sets, sets_size è la sua grandezza
	e sets_count è un array di interi che mi permette di contare le occorrenze
	di quell'insieme, una volta che ho unito due array in uno nuovo, controllo la presenza
	in sets se non c'è lo aggiungo, altrimenti incremento il contatore, se il contatore
	(il numero di celle in cui l'insieme compare) è uguale al numero di elementi nell'array
	allora ho trovato un preemptive set.

	Adesso devo capire cosa fare una volta che ne ho trovato uno
*/

/*

	ESEMPIO di preemptive set
	{[3, 4, 5, 6], [ c[3,0], c[3, 1], c[3, 4], c[4, 5] ]}

	in quanto facendo l'unione tra i possibili valori delle varie celle

	c[3, 0] : [3, 4, 5]
	c[3, 1] : [3, 4, 5]
	c[3, 4] : [3, 4, 5, 6]
	c[4, 5] : [4, 5, 6]

	quindi sono 4 valori per 4 celle.

	Adesso occorre cancellare per ogni cella nel preemptive set
	da tuttle le altre celle sulle righe, colonne e box 
	i valori presenti nella lista.

	IMPLEMENTA FIND PREEMPTIVE SET
*/

int sudoku[9][9] = {0, 0, 3, 0, 2, 0, 6, 0, 0,
		    		9, 0, 0, 3, 0, 5, 0, 0, 1,
		    		0, 0, 1, 8, 0, 6, 4, 0, 0,
		    		0, 0, 8, 1, 0, 2, 9, 0, 0,
		    		7, 0, 0, 0, 0, 0, 0, 0, 8,
		    		0, 0, 6, 7, 0, 8, 2, 0, 0,
		    		0, 0, 2, 6, 0, 9, 5, 0, 0,
		    		8, 0, 0, 2, 0, 3, 0, 0, 9,
		    		0, 0, 5, 0, 1, 0, 3, 0, 0};

int* markup_el[9][9];
cella* empty_cell;
int size_empty_cell = 0;
int markup_el_len[9][9][1]; 
set* sets;
int sets_size = 0;
int* sets_count;

/* 
   Una matrice in cui all'indirizzo i, j è memorizzata
   un vettore dei possibili valori che possono essere 
   inseriti nella cella i, j della matrice sudoku.
   														*/



int main(){

	print_sudoku();
	crook();
/*	for (int i = 0; i < size_empty_cell; i++){
		printf("c[%d, %d]\n", empty_cell[i].i, empty_cell[i].j);
	}*/
	return 0;
}

int crook(){
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			if (sudoku[i][j] == 0){
				to_consider(i, j);
				markup(i, j);
				//is_singleton(i, j);
			}
		}
	}
	find_preemptive_set(2, 0);

	return 0;
}

void find_preemptive_set(int m, int start){
	cella pivot = empty_cell[start];
	for (int i = start + 1; i < size_empty_cell; i++){
		cella other = empty_cell[i];
		unione(&pivot, &other, m);
		/*
			Per ogni cella controlla se ha un preemptive set con le altr
		*/
	}
}

void add_preemptive(cella* a, cella* b){

}

int unione(cella* a, cella* b, int m){
	/*
		Date 2 liste ne fa l'unione restituendo ancora non ho deciso cosa 
	*/
	//printf("A|c[%d, %d]\n", a->i, a->j);
	//printf("B|c[%d, %d]\n", b->i, b->j);
	//printf("SONO DENTRO UNIONE\n");
	int* possible_a = markup_el[a->i][a->j];
	int* possible_b = markup_el[b->i][b->j];
	int size_a = markup_el_len[a->i][a->j][0];
	int size_b = markup_el_len[b->i][b->j][0];
	//printf("POSSIBILI DI A: ");
	//print_vec(possible_a, size_a);
	//printf("\n");
	//printf("POSSIBILI DI B: ");
	//print_vec(possible_b, size_b);
	//printf("\n");
	int* z = NULL;
	int size_z = 0;
	int somma = 0;
	for (int i = 0; i < size_a; i++){
		z = realloc(z, (size_z + 1) * sizeof(int));
		z[size_z] = possible_a[i];
		somma += possible_a[i];
		size_z++;
	}
	for (int i = 0; i < size_b; i++){
		if (binarySearch(z, 0, size_z - 1, possible_b[i]) == -1){
			z = realloc(z, (size_z + 1) * sizeof(int));
			z[size_z] = possible_b[i];
			somma += possible_b[i];
			size_z++;
			//printf("size: %d\n", size_z);
		}
	}
	//printf("UNITI: ");
	//print_vec(z, size_z);
	//printf("\n");
	int idx = inPeemptiveSet(z, size_z, somma);
	if (idx != -1){
		sets_count[idx] += 1;
		sets[idx].celle = realloc(sets[idx].celle, (sets[idx].size_celle + 2) * sizeof(cella));
		sets[idx].celle[sets[idx].size_celle] = *a;
		sets[idx].celle[sets[idx].size_celle + 1] = *b;
		sets[idx].size_celle += 2;
		if (size_z == sets_count[idx]){
			print_preemptive(idx);
		}
	}
	else
		add_new_set(z, size_z, somma, a, b);
	return 0;


}

int inPeemptiveSet(int* arr, int size, int sum){
	/*
		Permette di scorrere l'array che contiene gli insiemi
		se l'id dell'insieme è uguale a sum ed anche la lunghezza è la stessa
		allora stiamo parlando dello stesso insieme.

		L'id è dato dalla somma degli elementi presenti all'interno della lista
		dunque (id, len) formano una chiave primaria, infatti identificano univocamente
		un insieme
	*/
	for (int i = 0; i < sets_size; i++){
		//printf("SET[i].id, sum: %d, %d\n", sets[i].id, sum);
		if (sets[i].id == sum && sets[i].len == size){
			//printf("TROVATO\n");
			return i;
		}
	}
	//printf("NON TROVATO\n");
	return -1;
}

void add_new_set(int* arr, int size, int id, cella* a, cella* b){
	set* new_set = malloc(sizeof(set));
	new_set->id = id;
	new_set->len = size;
	new_set->size_celle = 0;
	new_set->celle = NULL;
	new_set->array = arr;
	new_set->celle = realloc(new_set->celle, (new_set->size_celle + 2) * sizeof(cella));
	new_set->celle[new_set->size_celle] = *a;
	new_set->celle[new_set->size_celle + 1] = *b;
	new_set->size_celle += 2;
	//print_set(new_set);
	sets = realloc(sets, (sets_size + 1) * sizeof(set));
	sets_count = realloc(sets_count, (sets_size + 1) * sizeof(int));
	sets[sets_size] = *new_set;
	sets_count[sets_size] = 1;
	sets_size++;

}

int binarySearch(int* arr, int start, int end, int element){ 
	//printf("sto cercando: %d\n", element);
	//printf("start, end: %d, %d\n", start, end);
	//print_vec(arr, end);
    if (end >= start) { 
        int mid = start + (end - start) / 2; 
        if (arr[mid] == element) 
            return mid; 
        if (arr[mid] > element) 
            return binarySearch(arr, start, mid - 1, element); 
        return binarySearch(arr, mid + 1, end, element); 
    } 
    return -1; 
}

void to_consider(int i, int j){
	/*
		Inserisce nell'array empty_cell le celle che devo considerare per
		ottimizzare il calcolo dei preemptive sets
	*/
	empty_cell = realloc(empty_cell, (size_empty_cell + 1) * sizeof(cella));
	empty_cell[size_empty_cell].i = i;
	empty_cell[size_empty_cell].j = j;
	size_empty_cell++;

}

int markup(int i, int j){
	//printf("IN markup\n");
	int possibili[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	//printf("c[%d, %d]\n", i, j);
	check_row(i, j, possibili);
	check_col(i, j, possibili);
	check_box(i, j, possibili);
	//printf("POSSIBILI: ");
	//print_vec(possibili, 9);
	alloca_markup(i, j, possibili);
	//print_vec(markup_el[i][j], markup_el_len[i][j][0]);
	//printf("\n");
	return 0;
}

void is_singleton(int i, int j){
	if (markup_el_len[i][j][0] == 1){
		sudoku[i][j] = markup_el[i][j][0];
		print_sudoku();
	}
}

void alloca_markup(int i, int j, int* array){
	/*
		Versione leggermenta avanzata, dopo aver lavorato con array statici,
		quindi avendo array di lunghezza 9 con gli elementi cancellati settati a zero,
		tramite questa funzione assegno degli array dinamici per ogni cella,
		i quali contengono i possibili valori per quella cella.
	*/
	int size = 0;
	for (int x = 0; x < 9; x++){
		if (array[x] != 0){
			//printf("array[%d]: %d\n", x, array[x]);
			markup_el[i][j] = realloc(markup_el[i][j], (size + 1) * sizeof(int));
			markup_el[i][j][size] = array[x];
			size++;
		}
	}
	markup_el_len[i][j][0] = size;
}

void check_box(int i, int j, int* array){
	//printf("checking box\n");
	int local_i, local_j;
	local_i = get_index(i);
	local_j = get_index(j);

	//printf("[%d, %d]\n", local_i, local_j);

	for(int x = local_i; x < local_i + 3; x++){
		for (int y = local_j; y < local_j + 3; y++){
			if ( sudoku[x][y] != 0 )
				cancella(sudoku[x][y], array);
		}
	}
	//printf("exit checking box\n");

}

int get_index(int value){
	if (value < 3)
		return 0;
	if (value < 6)
		return 3;
	if (value < 9)
		return 6;
	return 1;
}

void check_row(int i, int j, int* array){
	//printf("checking row\n");
	for (i = i + 1; i < 9; i++){
		if (sudoku[i][j] != 0)
			cancella(sudoku[i][j], array);
	}
	//printf("exit checking row\n");
}

void check_col(int i, int j, int* array){
	//printf("checking col\n");
	for (j = j + 1; j < 9; j++){
		if (sudoku[i][j] != 0)
			cancella(sudoku[i][j], array);
	}
	//printf("exit checking col\n");
}

void cancella(int num, int* array){
	/*
		Versione basilare, infatti l'array non viene gestito dinamicamente
		e dunque per indicare che un elemento viene cancellato dai possibili
		viene settato a zero.
	*/
	//printf("deleting\n");
	//print_vec(array, 9);
	/*	
		IMPLEMENTA ARRAY DINAMICO CON RICERCA BINARIA
	*/
/*	int idx = binarySearch(array, 0, 9, num);
	if ( idx != -1){
		array[idx] = 0
	}*/
	for (int i = 0; i < 9; i++){
		if (num == array[i])
			array[i] = 0;
	}
}

void print_vec(int* array, int len){
	if (len >= 2){
		printf("[%d, ", array[0]);
		for (int i = 1; i < len - 1; i++)
			printf("%d, ", array[i]);
		printf("%d]", array[len - 1]);
	}
	else
		printf("[%d]\n", array[0]);
}

void print_sudoku(){
	for(int i = 0; i < 9; i++)
		printf(" ___");
	printf("\n");

	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			printf("| %d ", sudoku[i][j]);
		}
		printf("|\n");
	}

}

void print_set(set* insieme){
	printf("INSIEME\n");
	printf("id: %d\n", insieme->id);
	printf("len: %d\n", insieme->len);
	print_vec(insieme->array, insieme->len);
	printf("---------------\n");
}
void print_preemptive(int idx){
	int size = sets[idx].size_celle;
	cella* celle = sets[idx].celle;
	printf("PREEMPTIVE SET\n");
	printf("{");
	print_vec(sets[idx].array, sets[idx].len);
	printf(", ");
	if (size > 2){
		printf("[c[%d, %d], ", celle[0].i, celle[0].j);
		for (int i = 1; i < size - 1; i++){
			int x = celle[i].i;
			int y = celle[i].j;
			printf("c[%d, %d], ", x, y);
		}
		printf("c[%d, %d]", celle[size - 1].i, celle[size - 1].j);
	}
	else{
		printf("DEVI IMPLEMENTARLA\n");
	}
	printf("]}\n");
}