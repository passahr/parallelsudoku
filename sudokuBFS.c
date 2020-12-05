#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


typedef struct {
        int mat[3][3];
}box;


void print_sudoku(int[9][9]);
int is_col_valid(int i, int j, int num, int[9][9]);
int is_raw_valid(int i, int j, int num, int[9][9]);
int is_square_valid(int i, int j, int num, int[9][9]);
int num_perm_first_square(int i, int j, int* n);
int fill_base_square(int i, int j, box* p, int* n);
void calc_quads(int i, int j, box* p, int* n);
int resolve(int i, int j);

int sudoku2[9][9] = {0, 0, 3, 0, 2, 0, 6, 0, 0,
		    9, 0, 0, 3, 0, 5, 0, 0, 1,
		    0, 0, 1, 8, 0, 6, 4, 0, 0,
		    0, 0, 8, 1, 0, 2, 9, 0, 0,
		    7, 0, 0, 0, 0, 0, 0, 0, 8,
		    0, 0, 6, 7, 0, 8, 2, 0, 0,
		    0, 0, 2, 6, 0, 9, 5, 0, 0,
		    8, 0, 0, 2, 0, 3, 0, 0, 9,
		    0, 0, 5, 0, 1, 0, 3, 0, 0};


int sudoku[9][9] = {0, 0, 3, 0, 2, 0, 0, 0, 0,
                    9, 0, 0, 0, 0, 0, 0, 0, 1,
                    0, 0, 1, 0, 0, 0, 4, 0, 0,
                    0, 0, 0, 1, 0, 2, 9, 0, 0,
                    7, 0, 0, 0, 0, 0, 0, 0, 8,
                    0, 0, 6, 0, 0, 0, 2, 0, 0,
                    0, 0, 0, 0, 0, 9, 5, 0, 0,
                    8, 0, 0, 0, 0, 0, 0, 0, 9,
                    0, 0, 5, 0, 1, 0, 0, 0, 0};

int sudo[9][9] = {0, 0, 3, 0, 2, 0, 0, 0, 0,
                  9, 0, 0, 0, 0, 0, 0, 0, 1,
                  0, 0, 1, 0, 0, 0, 4, 0, 0,
                  0, 0, 0, 1, 0, 2, 9, 0, 0,
                  7, 0, 0, 0, 0, 0, 0, 0, 8,
                  0, 0, 6, 0, 0, 0, 2, 0, 0,
                  0, 0, 0, 0, 0, 9, 5, 0, 0,
                  8, 0, 0, 0, 0, 0, 0, 0, 9,
                  0, 0, 5, 0, 1, 0, 0, 0, 0};

int num_box=0;



int main(){
	clock_t t1,t2;
	print_sudoku(sudoku);
	t1=clock();

	
	num_perm_first_square(0, 0, &num_box);							// Funzione che mi mette nella variabile num_box il numero di permutazioni "legali" possibili del primo box in alto a sx
	printf("NUM BOX TOT : %d\n", num_box);
	box* lista_matrici_gen = malloc(num_box * sizeof(box));			// Creo una lista dove metterò tutte le combinazioni di primo box candidate
	num_box = 0;													
	fill_base_square(0, 0, lista_matrici_gen, &num_box);			// Riempio la lista con ogni primo box possibile per iniziare
	
	


	printf("NUM BOX TOT : %d\n", num_box);
	for(int b=0; b < num_box; b++){
		printf("PROVA CON IL BOX CANDIDATO N° %d\n", b);
		
		for(int i=0; i<3; i++)
	        printf(" ___");
		printf("\n");

		for(int i=0; i<8; i++)										// Reinizializzo il sudoku a quello iniziale
            	for(int j=0; j<8; j++)
					sudo[i][j] = sudoku[i][j];

    	for(int i=0; i<3; i++){										// Stampo il primo box candidato e metto i suoi valori nel sudoku
            	for(int j=0; j<3; j++){
            		sudo[i][j] = lista_matrici_gen[b].mat[i][j];
                   	printf("| %d ", lista_matrici_gen[b].mat[i][j]);
           		}
            	printf("|\n");
    	}
    	
    	if(resolve(0,0) == 1){										// Se quel box trova una soluzione legale allora la stampo ed esco
    		printf("\nSUDOKU RISOLTO\n");
			print_sudoku(sudo);
			break;
		}
		printf("IMPOSSIBILE RISOLVERE SUDOKU CON QUESTA COMBINAZIONE INIZIALE\n");			// Se il box non porta a una soluzione lo scrivo e nella prossima iterazione proverà con un altro box
    	

	}
	t2=clock();
	printf("Tempo di esecuzione = %d colpi di clock\n", t2-t1);
	return 0;
}


int num_perm_first_square(int i, int j, int* n){
	int num = 1;
	int temp_i, temp_j;
	if(sudoku[i][j] != 0){
		if(i == 2 && j == 2){
			*n= *n + 1;
			return 1;
		}
		if(j < 2)
			j++;
		else{
			j = 0;
			i ++;
		}
		return num_perm_first_square(i,j,n);
	}
	else{
		while(num < 10){
			if(is_col_valid(i, j, num, sudoku) && is_raw_valid(i, j, num, sudoku) && is_square_valid(i, j, num, sudoku)){
				sudoku[i][j]=num;
				if (i==2 && j==2){
					*n= *n + 1;
					return 1;
				}
				if(j < 2){
                    temp_j = j + 1;
					temp_i = i;
				}
            	else{
                    temp_j = 0;
                    temp_i = i + 1;
            	}
            	num_perm_first_square(temp_i, temp_j,n);
			}
			num++;
		}
		sudoku[i][j] = 0;
		return 0;
	}
}

int fill_base_square(int i, int j, box* p, int* n){
	int num = 1;
	int temp_i, temp_j;
	if(sudoku[i][j] != 0){
		if(i == 2 && j == 2){
			*n=*n+1;
			int app= *n;
			for(int x = 0; x < 3; x++)
				for(int y = 0; y < 3; y++){
					p[app - 1].mat[x][y]=sudoku[x][y];
				}
			return 1;
		}
		if(j < 2)
			j++;
		else{
			j = 0;
			i ++;
		}
		return fill_base_square(i,j,p,n);
	}
	else{
		while(num < 10){
			if(is_col_valid(i, j, num, sudoku) && is_raw_valid(i, j, num, sudoku) && is_square_valid(i, j, num, sudoku)){
				sudoku[i][j]=num;
				if (i==2 && j==2){
					*n=*n+1;
					int app= *n;
					for(int x = 0; x < 3; x++)
						for(int y = 0; y < 3; y++){
							p[app - 1].mat[x][y]=sudoku[x][y];
						}
				return 1;
				}
				if(j < 2){
                    temp_j = j + 1;
					temp_i = i;
				}
            	else{
                    temp_j = 0;
                    temp_i = i + 1;
            	}
            	fill_base_square(temp_i, temp_j,p,n);
			}
			num++;
		}
		sudoku[i][j] = 0;
		return 0;
	}
}


int resolve(int i, int j){
//	printf("I: %d, J: %d\n", i, j);
	int num = 1;
	int temp_i, temp_j;
	if(sudo[i][j] != 0){				// 0 indica la casella vuota
		if(i == 8 && j == 8)			// Se siamo nell'ultima casella e c'è già un numero il sudoku è risolto
			return 1;			// Ritorna 1 nel caso in cui il sudoku è risolto
		if(j < 8)				// Qui nel caso la casella sia occupata già si passa a quella sucessiva
			j++;
		else{
			j = 0;
			i ++;
		}
		return resolve(i,j);
	}
	else{
		while(num < 10){
			if(is_col_valid(i, j, num, sudo) && is_raw_valid(i, j, num, sudo) && is_square_valid(i, j, num, sudo)){
				sudo[i][j]=num;
				if (i==8 && j==8)
					return 1;
				if(j < 8){
                   	temp_j = j + 1;
					temp_i = i;
				}
                else{
               		temp_j = 0;
               		temp_i = i + 1;
           		}

				if(resolve(temp_i, temp_j) == 1)
					return 1;
			}
			num++;
		}
		sudo[i][j] = 0;
		return 0;

	}


}




/* Controllo che non sia giÃ  presente il numero nella posizione attuale in un'altra casella della stessa riga*/
int is_col_valid(int i, int j, int num, int sudoku[9][9]){
	for (int r=0; r<9; r++){
		if (r != i && sudoku[r][j] == num)
			return 0;
	}
	return 1;
}

/* Controllo che non sia giÃ  presente il numero nella posizione attuale in un'altra casella della stessa colonna*/
int is_raw_valid(int i, int j, int num, int sudoku[9][9]){
	for (int c=0; c<9; c++){
                if (c != j && sudoku[i][c] == num)
                        return 0;
        }
        return 1;
}

/* Controllo che non sia giÃ  presente il numero nella posizione attuale in un'altra casella all'interno dello stesso box di 9 caselle*/
int is_square_valid(int i, int j, int num, int sudoku[9][9]){
	int local_i, local_j;

	if(i < 3)					// Calcolo la i della casella nell'angolo in alto a sx del quadrato dove si trova la casella corrente
		local_i = 0;
	else
		if(i < 6)
			local_i = 3;
		else
			local_j = 6;

	if(j < 3)					// Calcolo la j della casella nell'angolo in alto a sx del quadrato dove si trova la casella corrente
		local_j = 0;
	else
		if(j < 6)
			local_j = 3;
		else
			local_j = 6;

	for(int x = local_i; x < local_i + 3; x++)
		for(int y = local_j; y< local_j + 3; y++)
			if ((x != i || y != j) && sudoku[x][y] == num)
				return 0;
	return 1;
}


/* Stampa del Sudoku*/
void print_sudoku(int sudoku[9][9]){
	for(int i=0; i<9; i++)
		printf(" ___");
	printf("\n");

	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			printf("| %d ", sudoku[i][j]);
		}
		printf("|\n");
	}

}
