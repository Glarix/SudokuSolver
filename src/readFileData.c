#include "../include/readFileData.h"



// A utility function to get maximum of two integers
int max (int a, int b) { return (a > b)? a: b; }

// Function to convert char to int
int charToInt(char toConvert){
    return toConvert - '0';
}

/**
 * The preprocessing function for Boyer Moore's
 * bad character heuristic
 * @param str the string to be found
 * @param size the size of the string
 * @param badchar the array for Boyer Moore's algorithm
**/
void badCharHeuristic( char *str, int size,
                        int badchar[NO_OF_CHARS])
{
    int i;
 
    // Initialize all occurrences as -1
    for (i = 0; i < NO_OF_CHARS; i++)
         badchar[i] = -1;
 
    // Fill the actual value of last occurrence
    // of a character
    for (i = 0; i < size; i++)
         badchar[(int) str[i]] = i;
}


/** 
 * A pattern searching function that uses Bad
 * Character Heuristic of Boyer Moore Algorithm 
 * @param txt the chunk of test to perform the search on
 * @param pat the text pattern that needs to be found in txt
 * @param firstOccurence a flag - 1 if you want to see only if pattern exists /
 * 2 if you want to see the index of first occurence / other if to see how many times
 * the pattern occures
 * @return 1 if flag set to 1 / index of first occurence if flag set 2 / nr. of occurences
 * if other flag value
**/
int search( char *txt,  char *pat, int firstOccurence)
{
    int ind;
    int count = 0;
    int m = strlen(pat);
    int n = strlen(txt);
 
    int badchar[NO_OF_CHARS];
 
    /* Fill the bad character array by calling
       the preprocessing function badCharHeuristic()
       for given pattern */
    badCharHeuristic(pat, m, badchar);
 
    int s = 0;  // s is shift of the pattern with
                // respect to text
    while(s <= (n - m))
    {
        int j = m-1;
 
        /* Keep reducing index j of pattern while
           characters of pattern and text are
           matching at this shift s */
        while(j >= 0 && pat[j] == txt[s+j])
            j--;
 
        /* If the pattern is present at current
           shift, then index j will become -1 after
           the above loop */
        if (j < 0)
        {
            count++;
            if (firstOccurence == 1)
                return 1; // patern exists in text
            else if(firstOccurence == 2){
                return s; // returns the index of first occurence
            }
            
            /* Shift the pattern so that the next
               character in text aligns with the last
               occurrence of it in pattern.
               The condition s+m < n is necessary for
               the case when pattern occurs at the end
               of text */
            ind = charToInt(txt[s+m]);
            s += (s+m < n)? m-badchar[ind] : 1;
            
        }
 
        else{
            /* Shift the pattern so that the bad character
               in text aligns with the last occurrence of
               it in pattern. The max function is used to
               make sure that we get a positive shift.
               We may get a negative shift if the last
               occurrence  of bad character in pattern
               is on the right side of the current
               character. */
            ind = charToInt(txt[s+j]);
            s += max(1, j - badchar[ind]);
        }
    }
    if(firstOccurence == 2)
        return -1; // if findIndex mode and no pattern found return index as -1
    
    return count;
}

/**
 * Function to get the response of a get request from a specified URL, and save it to a file
 * @param URLAddress the address on which the request is performed
**/
void get_html(char* URLAddress){

    CURL *curl = curl_easy_init();

    if(curl){
        FILE* fl = fopen("request2.txt", "w");
        // set options
        curl_easy_setopt(curl, CURLOPT_URL, URLAddress);
        curl_easy_setopt(curl, CURLOPT_FILE, fl);

         // perform action
        curl_easy_perform(curl);
        fclose(fl);
    }

    curl_easy_cleanup(curl);
}

/**
 * Function that finds the line which contains informations about the Sudoku board
 * @param readFileName name of the data file
**/ 
void findTableData(char* readFileName){

    FILE* fr = fopen(readFileName, "r");
    if(!fr){
        printf("Error at opening file fr in findTableData.\n");
        return;
    }
        
    
    char* line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fr) != -1)
    {
        char pat[] = "<DIV id=\"puzzle_container\""; // Pattern to look for in a line of characters
        int res = search(line, pat, 1);
        if (res) // if the pattern was found, the line gets written into the file
        {
            FILE* fl = fopen("foundData.txt", "w");
            if (!fl){
                printf("Error at opening file fl in findTableData.\n");
                fclose(fr);
                if (line)
                    free(line);
                return;
            }
                
            
            fwrite(line, len, 1, fl);
            fclose(fl);    
        }        
    }
    
    fclose(fr);
    if (line)
        free(line);

}

/**
 * Function to arange the Sudoku table data in a readable and easier to work way
 * @param readFileName the file that contains all the information about the Sudoku board
**/
void arrangeData(char* readFileName){
    
    FILE* fr = fopen(readFileName, "r");
    if(!fr){
        printf("Error at opening file fr in arrangeData.\n");
        return;
    }
    
    char* line = NULL;
    size_t len = 0;
    
    getline(&line, &len, fr); // gets the line of characters from file
    fclose(fr);


    FILE* fw = fopen(readFileName, "w");
    if(!fw){
        printf("Error at opening file fw in arrangeData.\n");
        fclose(fr);
        if (line)
            free(line);
        return;
    }
    
    char* buf = strtok(line, ">"); // creating the tokens separated by the ">" delimitator

    while (buf != NULL) // Writing all the tokens to file
    {
        fwrite(buf, strlen(buf), 1, fw);
        buf = strtok(NULL, ">");
        if(buf)
            fwrite(">\n", 2, 1, fw);
    }
    
    fclose(fw);
    if (line)
        free(line);
}


/**
 * Function to get all the given numbers for the Sudoku board from the file and storing
 * then into a char array
 * @param sudokuDataChar a char array that will hold all the known data for the Sudoku board
 * @param readFileName the name of the file that contains the data
**/
void getSudokuData(char sudokuDataChar[81], char* readFileName){
    
    int spotsCount = 0;
    FILE* fr = fopen(readFileName, "r");
    if(!fr){
        printf("Error at opening file fr in getSudokuData.\n");
        return;
    }
    
    char* line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fr) != -1)
    {
        int tr = search(line, "<TR>", 1); // If a line containing "<TR>" is found we analize a row of data for Sudoku board
        if (tr)
        {
            for(int i = 0; i < 9; i++)
            {
                getline(&line, &len, fr); // <TD> LINE
                getline(&line, &len, fr); // <INPUT> LINE
                //int index = search(line, "READONLY VALUE=", 2); // Call of the search function with mode 2 which returns the index of the pattern occurence
                // For some reason the line above is not working ok anymore so hardcoded aproach
                int index = -1;
                // If a line has the letter 'R' at index 54, it contains information about a number on the board
                if (line[54] == 'R')
                {
                    index = 54;
                }
                
                getline(&line, &len, fr); // <TD> LINE
                
                if (index != -1){ // If a value is found, it is stored in the char array
                    sudokuDataChar[spotsCount] = line[index + 16]; // to get the value we need to add 16 to the starting index of found pattern
                }
                else{
                    sudokuDataChar[spotsCount] = '0'; // If there is no data for the specific square it's value in array is set to '0'
                }                
                spotsCount++;
            }
        }    
    }

    fclose(fr);
    if (line)
        free(line);
}

/**
 * Function to convert the char array of data to int data and fill a Sudoku board matrix
 * @param sudokuDataChar char array containing all the Sudoku board data to be converted
 * @param sudokuDataIntBoard int matrix that will represent the Sudoku board
**/
void charArrToIntMatrix(char sudokuDataChar[81], int sudokuDataIntBoard[9][9]){
    int count = 0;
    for (int row = 0; row < 9; row++)
        for (int col = 0; col < 9; col++)
        {
            int data = charToInt(sudokuDataChar[count]); // Converting char to int 
            sudokuDataIntBoard[row][col] = data; // Assigning the value to a Sudoku cell
            count++;
        }    
}


/**
 * Function to concatenate two char*'s
 * @param s1 the first char*
 * @param s2 the char* to be concatenated to s1
 * @return the the concatenated string
**/
char* concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    if(!result){
        return NULL;
    }
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/**
 * Function to get the informations about the requested table of SUDOKU
 * @param URL the char* containing the full URL which contains the data for the table
 * @param matrix the int matrix that will store all information necessary for the game
**/
void getRequestedTable(char* URL, int matrix[9][9]){
    
    char sudokuDataChar[81] = {"0"};
    
    get_html(URL);
    findTableData("request2.txt");
    arrangeData("foundData.txt");
    getSudokuData(sudokuDataChar, "foundData.txt");
    charArrToIntMatrix(sudokuDataChar, matrix);
}

void loadLevelFromDb(char* path, int matrix[9][9]){
    char sudokuDataChar[81] = {"0"};
    getSudokuData(sudokuDataChar, path);
    charArrToIntMatrix(sudokuDataChar, matrix);
}
