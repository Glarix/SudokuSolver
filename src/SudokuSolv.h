

#ifndef __SUDOKUSOLV__
#define __SUDOKUSOLV__

/**
 * Fuction to find the first empty spot on a board (value 0)
 * @param board[9][9] the Sudoku board to be checked
 * @param iPos address that will contain the row of the empty spot
 * @param jPos address that will contain the column of the empty spot
 * @return 0 if no empty spaces / 1 if empty space found
*/
int findEmptySpot(int board[9][9], int* iPos, int* jPos);

/**
 * Function to check if a number is valid to be placed in that position
 * @param board - the board to check
 * @param row - the row coordinate
 * @param col - the column coordinate
 * @param num - number to be checked if it's valid
 * @return - 1 if it's valid / 0 if not
*/
int valid(int board[9][9], int row, int col, int num);

/**
 * Function that solves a Sudoku board using backtracking and recursion
 * @param board[9][9] a unsolved Sudoku board
 * @return 1 if table is solvable / 0 if it's imposible to solve
*/
int Solve(int board[9][9]);

#endif