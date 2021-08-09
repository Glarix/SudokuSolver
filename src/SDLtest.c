#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>

#include "SudokuSolv.h"
#include "menu.h"
#include "readFileData.h"


const int WIDTH = 600;
const int HEIGHT = 600;
const int ROW = 9;
const int COL = 9;

int finishFlag = 0;

// Structure of a square from the Sudoku board
typedef struct sudokuSquare
{
    int value;               // numerical value of suSquare
    bool isSet;              // if it is set
    bool isSelectable;       // if isSet then is not selectable
    bool isSelected;         // if is selected
    SDL_Texture *numTexture; // pointer to texture of number
    SDL_Rect suSquare;       // the rectangle struct for specific square
} SudSquare;



int int_rand(){//function to generate a random time period for sleep
	int min = 21, max = 100000;
    int num = (rand() % (max - min + 1)) + min;
    return num;
}


/**
 * Function to initialise the window and renderer 
 * @param window a pointer to a SDL_Window pointer to be initialised
 * @param renderer the SDL_Renderer pointer to be initialised
 * @return 0 if initialization succesful / 1 if something went wrong
**/
int initSDL(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) // Initializing SDL subsystems
    {
        printf("Error at init: %s\n", SDL_GetError());
        return 1;
    }

    // Initializing window
    *window = SDL_CreateWindow("~SUDOKU~",             // Window name
                               SDL_WINDOWPOS_CENTERED, // x position
                               SDL_WINDOWPOS_CENTERED, // y position
                               WIDTH, HEIGHT, 0);      // width, heigth, flags

    if (!(*window)) // if initialization failed
    {
        printf("Error at init window %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Flags for the renderer
    Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    *renderer = SDL_CreateRenderer(*window, -1, renderFlags); // Initializing renderer

    if (!(*renderer)) // if initialization failed
    {
        printf("Error at init renderer\n");
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return 1;
    }
    // Setting renderer to blend mode
    SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);

    return 0;
}

/**
 * Function to load the board from a int matrix, it sets up every SudSquare of the board
 * @param board[ROW][COL] a matrix of SudSquares that represents the board
 * @param intBoard[ROW][COL] a matrix of ints that represents the sudoku game
 * @param renderer the SDL_Renderer used to render the window
 * @param font a pointer to the loaded font
 * @return 0 after every initialisation is complete
**/
int loadBoard(SudSquare board[ROW][COL], int intBoard[ROW][COL], SDL_Renderer *renderer, TTF_Font *font)
{

    //SDL_SetRenderDrawColor(renderer, 245, 220, 150, 150);
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            SDL_Color textColor = {255, 255, 255, 0}; // Color for the font

            // Setting up the rect for each Square of the board
            board[i][j].suSquare.x = j * 60 + 30;
            board[i][j].suSquare.y = i * 60 + 30;
            board[i][j].suSquare.w = 60;
            board[i][j].suSquare.h = 60;

            board[i][j].value = intBoard[i][j]; // Set the value
            board[i][j].isSelected = false;     // Set the is Selected flat to false
            if (intBoard[i][j] == 0)
            {
                board[i][j].isSet = false;       // The square is not set
                board[i][j].isSelectable = true; // The square is selectable
            }
            else
            {
                board[i][j].isSet = true;
                board[i][j].isSelectable = false;
                finishFlag++; // Update finishFlag
            }
            SDL_Surface *surface; // Surface pointer
            char msg[5];          // array for storing the value in char form
            sprintf(msg, "%d", intBoard[i][j]);
            surface = TTF_RenderText_Solid(font, msg, textColor);                     // Creating the surface
            board[i][j].numTexture = SDL_CreateTextureFromSurface(renderer, surface); // Creating the texture
            SDL_FreeSurface(surface);                                                 // Destroying the surface
        }
    }
    return 0;
}

/**
 * Function to Copy the Textures for numbers to the back buffer to be shown 
 * @param board the Sudoku board containing all the squares
 * @param renderer the SDL_Renderer used to render the window
**/
void renderNumbers(SudSquare board[ROW][COL], SDL_Renderer *renderer)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            // if the square isSet then load the texture to the back buffer
            if (board[i][j].isSet)
            {
                SDL_RenderCopy(renderer, board[i][j].numTexture, NULL, &(board[i][j].suSquare));
            }
        }
    }
}

/**
 * Function that clears the Active state of every tile
 * @param board the Sudoku board containing all the squares
**/
void clearActivity(SudSquare board[ROW][COL])
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (board[i][j].isSelected)
            {
                board[i][j].isSelected = false;
                if (!(board[i][j].isSet))
                    board[i][j].isSelectable = true;
            }
        }
    }
}

/**
 * Function to check the mouse button that was pressed and to set the isSelected property of a square
 * @param board the Sudoku board containing all the squares
 * @param event the SDL_Event that triggered the call to this function
 * @param i the ROW of the square in board
 * @param j the COL of the square in board
 * @return true if the square got selected / false if not selected
**/
bool checkMouseEvent(SudSquare board[ROW][COL], SDL_Event event, int i, int j)
{
    // checking if the click was in range and if left button was pressed
    if (!(i < 0 || j < 0) && event.button.button == SDL_BUTTON_LEFT)
    {
        board[i][j].isSelected = true;
        return true;
    }
    else if (!(i < 0 || j < 0) && event.button.button == SDL_BUTTON_RIGHT)
    {
        board[i][j].isSelected = false;
        return false;
    }
    return false;
}

/** 
 * Function to highlight a square that is not Set but is Selected
 * @param board the Sudoku board containing all the squares
 * @param renderer the SDL_Renderer used to render the window
**/
void checkSelected(SudSquare board[ROW][COL], SDL_Renderer *renderer)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (board[i][j].isSelected && !board[i][j].isSet)
            {
                // Set the color to a yellow-ish one
                SDL_SetRenderDrawColor(renderer, 245, 220, 150, 150);
                // Fill the square
                SDL_RenderFillRect(renderer, &(board[i][j].suSquare));
            }
        }
    }
}

/**
 * Function to draw the board and the squares for the game
 * @param renderer a pointer to the SDL_Renderer used to render the window
 * @param rect the first rect wrapping the board
 * @param rect2 the second rect wrapping the board 
 * @param board the Sudoku board containing all the squares
**/
void drawBoard(SDL_Renderer *renderer, SDL_Rect rect, SDL_Rect rect2, SudSquare board[ROW][COL])
{
    int mod = 1;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Making the screen black
    SDL_RenderClear(renderer);

    // Function to render the textures of the Set squares to the back buffer
    renderNumbers(board, renderer);

    // Function to highlight a square that is not Set but is Selected
    checkSelected(board, renderer);

    // Setting the draw collor to purple-ish
    SDL_SetRenderDrawColor(renderer, 200, 155, 250, 255);

    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderDrawRect(renderer, &rect2);

    //Horiontal lines
    for (int i = 90; i < 540; i += 60)
    {
        if (mod % 3 == 0)
        {
            SDL_RenderDrawLine(renderer, 30, i - 1, 569, i - 1);
            SDL_RenderDrawLine(renderer, 30, i + 1, 569, i + 1);
        }
        mod++;
        SDL_RenderDrawLine(renderer, 30, i, 569, i);
    }
    mod = 1;

    //Vertical lines
    for (int i = 90; i < 540; i += 60)
    {
        if (mod % 3 == 0)
        {
            SDL_RenderDrawLine(renderer, i - 1, 30, i - 1, 569);
            SDL_RenderDrawLine(renderer, i + 1, 30, i + 1, 569);
        }
        mod++;
        SDL_RenderDrawLine(renderer, i, 30, i, 569);
    }
    mod = 1;

    // Swithing the front buffer with the back buffer
    SDL_RenderPresent(renderer);
}

/**
 * Function to clean the textures of every board Square
 * @param board the Sudoku board containing all the squares
 * @return 0 memory freed successfuly
**/
int cleanBoard(SudSquare board[ROW][COL])
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            SDL_DestroyTexture(board[i][j].numTexture);
            board[i][j].numTexture = NULL;
        }
    }
    return 0;
}

/**
 * Function to determine which square was selected
 * @param i address that will store the COL of the square
 * @param j address that will store the ROW of the square
 * @param x the x mouse coordinates
 * @param y the y mouse coordinates
**/
void getCellByMouse(int *i, int *j, int x, int y)
{
    // return -1 if clicked out of table
    if (x < 30 || y < 30)
    {
        *i = -1;
        *j = -1;
        return;
    }

    if (x > 570 || y > 570)
    {
        *i = -1;
        *j = -1;
        return;
    }
    // Calculating the COL and ROW of square
    *i = (x - 30) / 60;
    *j = (y - 30) / 60;
}

/**
 * Function to set the given number in the chosen square
 * @param num the int number to be setted in the square
 * @param board the Sudoku board containing all the squares
 * @param i the ROW of the square in board
 * @param j the COL of the square in board
 * @param rend a pointer to the SDL_Renderer used to render the window
 * @param font a pointer to the loaded font
 * @param textColor the SDL_Color used for the font
 * @return 0 if the number was set succesfully / 1 if failed
**/
int placeNumber(int num, SudSquare board[ROW][COL], int i, int j, SDL_Renderer *rend, TTF_Font *font, SDL_Color textColor)
{
    if (!(board[i][j].isSet))
    {
        board[i][j].value = num;
        board[i][j].isSelected = false;
        board[i][j].isSelectable = false;
        board[i][j].isSet = true;
        SDL_Surface *surface; // Surface pointer
        char msg[5];          // array for storing the value in char form
        sprintf(msg, "%d", num);
        surface = TTF_RenderText_Solid(font, msg, textColor); // Creating the surface
        if (!surface)
        {
            printf("Error at creating surface in placeNumber!\n");
            return 1;
        }
        board[i][j].numTexture = SDL_CreateTextureFromSurface(rend, surface); // Creating the texture
        if (!(board[i][j].numTexture))
        {
            printf("Error at creating texture in placeNumber!\n");
            SDL_FreeSurface(surface);
            return 1;
        }

        SDL_FreeSurface(surface);
    }

    return 0;
}

/**
 * Function to auto-complete all the remaining Squares
 * @param matrix a pre-solved matrix containing all the data for Sudoku board
 * @param board the Sudoku board containing all the squares
 * @param rend the SDL_Renderer used to render the window
 * @param font a pointer to the loaded font
**/
void autoComplete(int matrix[ROW][COL], SudSquare board[ROW][COL], SDL_Renderer *rend, TTF_Font *font)
{
    //sets the color to pink-ish
    SDL_Color textColor = {237, 52, 135, 0};
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (!(board[i][j].isSet))
            {
                // If the Square is not Set, set the coresponding number and update finishFlag
                placeNumber(matrix[i][j], board, i, j, rend, font, textColor);
                finishFlag++;
            }
        }
    }
}

int main()
{
    srand(time(NULL));
    // Variables to store the level and difficulty of the chosen game
    int difficulty = -1, level = -1;
    
    // int matrix[9][9] = {
    //     {9, 1, 0, 7, 0, 0, 0, 0, 0},
    //     {0, 3, 2, 6, 0, 9, 0, 8, 0},
    //     {0, 0, 7, 0, 8, 0, 9, 0, 0},
    //     {0, 8, 6, 0, 3, 0, 1, 7, 0},
    //     {3, 0, 0, 0, 0, 0, 0, 0, 6},
    //     {0, 5, 1, 0, 2, 0, 8, 4, 0},
    //     {0, 0, 9, 0, 5, 0, 3, 0, 0},
    //     {0, 2, 0, 3, 0, 1, 4, 9, 0},
    //     {0, 0, 0, 0, 0, 2, 0, 6, 1}};

    int matrix[9][9] = {0};

    // The window that is displayed on screen
    SDL_Window *window = NULL;

    // The renderer of the screen
    SDL_Renderer *renderer = NULL;

    // Initialising the SDL subsystems, window and renderer
    int badInit = initSDL(&window, &renderer);
    if (badInit)
        return 1;

    // Initialising TTF
    if (TTF_Init())
    {
        printf("Error at initialising TTF! %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialising a font
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 12);
    if (font == NULL)
    {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }

    // Function to open the menu and select game
    generateMenu(font, renderer, &difficulty, &level);

    char diff[5], lvl[8];
    char link[37] = "https://nine.websudoku.com/?level=";
    char link2[10] = "&set_id=";
    char* firstConcat = NULL;
    if(level == -1){
        int randLvl = int_rand();
        sprintf(lvl, "%d", randLvl);
    }else{
        sprintf(lvl, "%d", level);    
    }
    sprintf(diff, "%d", difficulty);
    firstConcat = concat(link, diff);
    firstConcat = concat(firstConcat, link2);
    firstConcat = concat(firstConcat, lvl);

    getRequestedTable(firstConcat, matrix);
    

    // The Sudoku bord consisting of Square structs
    SudSquare board[ROW][COL];

    // loading the initial board with the renderer
    loadBoard(board, matrix, renderer, font);

    Solve(matrix);

    // the two rects that go around the board
    SDL_Rect rect = {30, 30, 540, 540};
    SDL_Rect rect2 = {29, 29, 539, 539};

    SDL_Event event;

    // While true the game is running
    bool running = true;
    // Flag to determine if a SudSquare is selected
    bool selectedSquare = false;
    
    
    // Main game loop
    while (running)
    {
        // Square coordinates
        int i, j;
        while (SDL_PollEvent(&event))
        {
            // Quit event handeler
            if (event.type == SDL_QUIT)
                running = false;
            // MouseButton event handler
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int xCoord, yCoord;
                // Clear the activity of every board square
                clearActivity(board);

                SDL_GetMouseState(&xCoord, &yCoord);

                // Getting the Square coordinates based on the mouse position
                getCellByMouse(&i, &j, yCoord, xCoord);

                // Checking if it was leftClick or rightClick
                selectedSquare = checkMouseEvent(board, event, i, j);
            }
            // KeyPressed event handler
            else if (event.type == SDL_KEYDOWN)
            {
                if (selectedSquare)
                {
                    for (int k = 1; k < 10; k++)
                    {
                        if (event.key.keysym.sym == SDLK_0 + k)
                        {
                            if (!checkMove(k, matrix, i, j))
                            {
                                SDL_Color textColor = {0, 255, 0, 0};
                                // Getting the number in the specific square
                                placeNumber(k, board, i, j, renderer, font, textColor);
                                finishFlag++;
                                break;
                            }
                            else
                            {
                                printf("Invalid Move!\n");
                                board[i][j].isSelected = false;
                                board[i][j].isSelectable = true;
                            }
                        }
                    }
                }

                // If one of the enter keys is pressed the game autocompletes and ends
                if (event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN)
                {
                    // Function that completes the remaining Squares
                    autoComplete(matrix, board, renderer, font);
                }
            }
        }
        // Drawing the board with everithing loaded in
        drawBoard(renderer, rect, rect2, board);

        // Checking if all the Sudoku Squares are completed and ends the game cycle
        if (finishFlag == 81)
        {
            printf("Game Complete!\n");
            running = false;
        }
    }

    free(firstConcat);
    // Delay of 5 seconds after game ended
    SDL_Delay(1000);
    cleanBoard(board);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}