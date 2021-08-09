#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#ifndef __READFILE__
#define __READFILE__

# define NO_OF_CHARS 256

char* concat(const char *s1, const char *s2);

void getRequestedTable(char* URL, int matrix[9][9]);

#endif