/*btSolve function will be our main function, it'll mantain its recursive structure, the true parallelization
 will be implemented inside the check function, since it'll be called much more times and it fits pretty well in a
 parallel implementation*/
//minor bug in check function, almost working.

#include <stdio.h>

#include "btsudoku.h"

int checkValue(int matrix[9][9], int x, int y, int i)
{
    //black magic will happen here
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
        if(matrix[(y/3)*3+ (q/3)][(x/3)*3+(q%3)] == i)//square expression
        {
            numInSqu++;
        }
    }
    if(numInRow > 0)
    {
        return 0;
    }
    if(numInCol > 0)
    {
        return 0;
    }
    if(numInSqu > 0)
    {
        return 0;
    }
    return 1;
}

int btSolve(int matrix[9][9], int x, int y)
{
    if(x<9 && y<9)//checks if sudoku table is not over
    {
        int cell = matrix[y][x]; //identifies a cell as an xy-combination
        int i; //value of the cell

        if(cell == 0) //if value is yet to be defined
        {
            for(i = 0; i<9; i++) //for every possible values: 1-9
            {
                
                if(checkValue(matrix, x, y, i+1))//check if is possible to insert value
                {
                    //if it is, insert
                    matrix[y][x] = i+1; 
                    if(x+1<9) //if not the last element in row
                    { 
                        if(btSolve(matrix, x+1, y))
                        {
                            return 1;
                        }
                        else
                        {
                            matrix[y][x] = 0;
                        }
                                       
                    }
                    else if(y+1<9) //if it's the last element in row but not the very last element
                    { 
                        if(btSolve(matrix, 0, y+1))
                        {
                            return 1;
                        }
                        else
                        {
                            matrix[y][x] = 0;
                        }
                        
                    }
                    else return 1;
                }
                
            }
            return 0;
        }
        else // SKIP 
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