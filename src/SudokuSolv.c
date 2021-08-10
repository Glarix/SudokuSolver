#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/SudokuSolv.h"
/**
 * Fuction to find the first empty spot on a board (value 0)
 * @param board[9][9] the Sudoku board to be checked
 * @param iPos address that will contain the row of the empty spot
 * @param jPos address that will contain the column of the empty spot
 * @return 0 if no empty spaces / 1 if empty space found
*/
int findEmptySpot(int board[9][9], int* iPos, int* jPos){

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if ( board[i][j] == 0)
            {
                *iPos = i;
                *jPos = j;
                return 1;
            }
    return 0;
}


/**
 * Function to check if a number is valid to be placed in that position
 * @param board - the board to check
 * @param row - the row coordinate
 * @param col - the column coordinate
 * @param num - number to be checked if it's valid
 * @return - 1 if it's valid / 0 if not
*/
int valid(int board[9][9], int row, int col, int num){
    int i = row;
    int j = col;

    //checking if is valid in row
    for ( int z = 0; z < 9; z++)
    {
        if ( board[i][z] == num && z != j)
            return 0;
    }

    //checking if is valid in column
    for (int z = 0; z < 9; z++)
    {
        if ( board[z][j] == num && z != i)
            return 0;
    }

    //checking if is valid in box
    int x = i / 3;
    int y = j / 3;
    int newX = x * 3, newY = y * 3;

    for ( int z = newX;  z < newX + 3; z++)
        for (int w = newY; w < newY + 3; w++)
            if ( board[z][w] == num && z != i && w != j)
                return 0;
            
    //if a number is valid
    return 1;
}

/**
 * Function that solves a Sudoku board using backtracking and recursion
 * @param board[9][9] a unsolved Sudoku board
 * @return 1 if table is solvable / 0 if it's imposible to solve
*/
int Solve(int board[9][9]){

    int i = 0;
    int j = 0;
    
    if (findEmptySpot(board, &i, &j) == 0)
        return 1;
    

    //int i = pos[0], j = pos[1];
    for (int num = 1; num < 10; num++)
    {
        
        int Valid = valid(board, i, j, num);
        if (Valid)
        {
            // If a number is valid on the given position it is atributed 
            board[i][j] = num;
        
            // Trying to Solve the board recursevly from now on
            if (Solve(board))
                return 1;
        }
        // If the number did not lead to a solution, that position resets to 0
        board[i][j] = 0;
    }
    return 0;
}  

/**
 * Function to check if a move is valid or not based on an already solved board
 * @param num the number to be checked
 * @param board the already solved matrix
 * @param row the row to be checked
 * @param col the column to be checked
 * @return 0 if the number is valid / 1 if invalid
**/
int checkMove(int num, int board[9][9], int row, int col){

    if (board[row][col] == num)
        return 0;
    return 1;
    
}