#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#ifndef __READFILE__
#define __READFILE__

# define NO_OF_CHARS 256


/**
 * Function to concatenate two char*'s
 * @param s1 the first char*
 * @param s2 the char* to be concatenated to s1
 * @return the the concatenated string
**/
char* concat(const char *s1, const char *s2);


/**
 * Function to get the informations about the requested table of SUDOKU
 * @param URL the char* containing the full URL which contains the data for the table
 * @param matrix the int matrix that will store all information necessary for the game
**/
void getRequestedTable(char* URL, int matrix[9][9]);


void loadLevelFromDb(char* path, int matrix[9][9]);
#endif

