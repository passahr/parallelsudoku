#include <stdio.h>
#include <time.h>

#include "btsudoku.h"

clock_t start, end;
double cpu_time_used;


int main()
{
    int sudoku[9][9]={  
                        {0, 0, 0, 0, 0, 0, 0, 9, 0},
                        {1, 9, 0, 4, 7, 0, 6, 0, 8},
                        {0, 5, 2, 8, 1, 9, 4, 0, 7},
                        {2, 0, 0, 0, 4, 8, 0, 0, 0},
                        {0, 0, 9, 0, 0, 0, 5, 0, 0},
                        {0, 0, 0, 7, 5, 0, 0, 0, 9},
                        {9, 0, 7, 3, 6, 4, 1, 8, 0},
                        {5, 0, 6, 0, 8, 1, 0, 7, 4},
                        {0, 8, 0, 0, 0, 0, 0, 0, 0}};
    
    int sudoku2[9][9] = {
                            {0,0,0,2,0,0,0,0,6},
                            {0,0,0,0,0,0,0,0,9},
                            {0,0,4,0,0,0,1,0,0},
                            {0,9,0,0,0,0,0,0,4},
                            {0,0,0,0,0,6,0,0,0},
                            {1,0,0,0,7,0,0,0,5},
                            {0,0,3,0,0,0,2,0,0},
                            {5,0,0,0,0,0,0,6,0},
                            {4,0,0,6,0,0,0,0,1}};
    
    int grid[9][9] = {  
                        {3, 0, 6, 5, 0, 8, 4, 0, 0}, 
                        {5, 2, 0, 0, 0, 0, 0, 0, 0}, 
                        {0, 8, 7, 0, 0, 0, 0, 3, 1}, 
                        {0, 0, 3, 0, 1, 0, 0, 8, 0}, 
                        {9, 0, 0, 8, 6, 3, 0, 0, 5}, 
                        {0, 5, 0, 0, 9, 0, 6, 0, 0}, 
                        {1, 3, 0, 0, 0, 0, 2, 5, 0}, 
                        {0, 0, 0, 0, 0, 0, 0, 7, 4}, 
                        {0, 0, 5, 2, 0, 6, 3, 0, 0} };
    
    start = clock();
    if(btSolve(sudoku, 0, 0))
    {
        int i;
        int j;
        printf("\n+-----+-----+-----+\n");
        for(i=1; i<10; ++i)
        {
            for(j=1; j<10; ++j) printf("|%d", sudoku[i-1][j-1]);
            printf("|\n");
            if (i%3 == 0) printf("+-----+-----+-----+\n");
        }
    }
    else printf("\n\nNO SOLUTION\n\n");

    if(btSolve(grid, 0, 0))
    {
        int i;
        int j;
        printf("\n+-----+-----+-----+\n");
        for(i=1; i<10; ++i)
        {
            for(j=1; j<10; ++j) printf("|%d", grid[i-1][j-1]);
            printf("|\n");
            if (i%3 == 0) printf("+-----+-----+-----+\n");
        }
    }
    else printf("\n\nNO SOLUTION\n\n");

    if(btSolve(sudoku2, 0, 0))
    {
        int i;
        int j;
        printf("\n+-----+-----+-----+\n");
        for(i=1; i<10; ++i)
        {
            for(j=1; j<10; ++j) printf("|%d", sudoku2[i-1][j-1]);
            printf("|\n");
            if (i%3 == 0) printf("+-----+-----+-----+\n");
        }
    }
    else printf("\n\nNO SOLUTION\n\n");

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("it took %f seconds to execute", cpu_time_used);
    return 0;
}