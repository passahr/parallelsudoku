all: sudoku_crook_serial sudoku_crook_parallel

sudoku_crook_serial: ./src/serialAlgos/sudoku_crook.c   ./src/datastructures/arraylist.c ./src/datastructures/preemptive_set.c ./src/datastructures/cellist.c ./src/datastructures/stack.c
	gcc -Wall  ./src/serialAlgos/sudoku_crook.c  ./src/datastructures/arraylist.c ./src/datastructures/preemptive_set.c ./src/datastructures/cellist.c ./src/datastructures/stack.c -o ./bin/sudoku_crook_serial

sudoku_crook_parallel: ./src/parallelAlgos/sudoku_crook.c   ./src/datastructures/arraylist.c ./src/datastructures/preemptive_set.c ./src/datastructures/cellist.c ./src/datastructures/stack.c
	gcc -Wall  ./src/parallelAlgos/sudoku_crook.c  ./src/datastructures/arraylist.c ./src/datastructures/preemptive_set.c ./src/datastructures/cellist.c ./src/datastructures/stack.c  -o ./bin/sudoku_crook_parallel -lpthread