/*btSolve function will be our main function, it'll mantain its recursive structure, the true parallelization
 will be implemented inside the check function, since it'll be called much more times and it fits pretty well in a
 parallel implementation*/
//minor bug in check function, almost working.

#include <stdio.h>

int btSolve(int matrix[9][9], int x, int y)
{
    if(x<9 && y<9)//checks if sudoku tabel is not over
    {
        int cell = matrix[y][x]; //identifies a cell as an xy-combination
        int i; //value of the cell

        if(cell == 0)
        {
            for(int i = 1; i<10; i++)
            {
                matrix[y][x] = i;
                if(check(matrix, x, y, i))
                { 
                    if(x+1<9)
                    { 
                        if(btSolve(matrix, x+1, y))
                        {
                            return 1;
                        }               
                    }
                    else if(y+1<9)
                    { 
                        if(btSolve(matrix, 0, y+1))
                        {
                            return 1;
                        }
                    }
                    else return 1;
                }
                
            }
            return 0;
        }
        else
        {
            if(x+1<9)
            { 
                return btSolve(matrix, x+1, y);
            }
            else if(y+1<9)
            { 
                return btSolve(matrix, 0, y+1);
            }
            else return 1;
        }
    }
}

int check(int matrix[9][9], int x, int y, int i)
{
    int numInCol = 0;
    int numInRow = 0;
    int numInSqu = 0;
    for(int q = 0; q<9; q++)
    {
        if(matrix[y][q] == i)
        {
            numInRow++;
        }
        if(matrix[q][x] == i)
        {
            numInCol++;
        }
        if(matrix[(x/3)*3+(q%3)][(y/3)*3+ (q/3)] == i)
        {
            numInSqu++;
        }
    }
    if(numInRow > 1)
    {
        return 0;
    }
    if(numInCol > 1)
    {
        return 0;
    }
    if(numInSqu > 1)
    {
        return 0;
    }
    return 1;
}

int main()
{
    int sudoku[9][9]={{0, 0, 0, 0, 0, 0, 0, 9, 0},
                  {1, 9, 0, 4, 7, 0, 6, 0, 8},
                  {0, 5, 2, 8, 1, 9, 4, 0, 7},
                  {2, 0, 0, 0, 4, 8, 0, 0, 0},
                  {0, 0, 9, 0, 0, 0, 5, 0, 0},
                  {0, 0, 0, 7, 5, 0, 0, 0, 9},
                  {9, 0, 7, 3, 6, 4, 1, 8, 0},
                  {5, 0, 6, 0, 8, 1, 0, 7, 4},
                  {0, 8, 0, 0, 0, 0, 0, 0, 0}};
    
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
    return 0;
}