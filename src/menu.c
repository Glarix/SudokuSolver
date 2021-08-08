#include "menu.h"
/**
 * Function to prepare the needed buttons to be displayed on the menu
 * @param randomButton a MenuButton that will represent the Random option button
 * @param chooseButton a MenuButton that will represent the Choose option button
 * @param level an array containing 20 MenuButton structs to represent levels
 * @param font the font used to render text on the window
 * @param rend the SDL_Renderer used to render the window
**/
void createMenuButtons(MenuButton *randomButton, MenuButton *chooseButton, MenuButton level[20], TTF_Font *font, SDL_Renderer *rend)
{
    SDL_Color color = {255, 255, 255, 255};
    // Creating random button
    randomButton->rect.x = 100;
    randomButton->rect.y = 285;
    randomButton->rect.w = 100;
    randomButton->rect.h = 30;
    SDL_Surface *surf;
    surf = TTF_RenderText_Solid(font, "Random", color);
    randomButton->texture = SDL_CreateTextureFromSurface(rend, surf);
    SDL_FreeSurface(surf);

    // Creating choose button
    chooseButton->rect.x = 400;
    chooseButton->rect.y = 285;
    chooseButton->rect.w = 100;
    chooseButton->rect.h = 30;
    SDL_Surface *surf2;
    surf2 = TTF_RenderText_Solid(font, "Choose", color);
    chooseButton->texture = SDL_CreateTextureFromSurface(rend, surf2);
    SDL_FreeSurface(surf2);

    // Creating the 20 level buttons
    int count = 0, x = 30, y = 30;
    for (int i = 0; i < 20; i++)
    {
        level[i].rect.x = x;
        level[i].rect.y = y;
        level[i].rect.w = 30;
        level[i].rect.h = 30;

        SDL_Surface *surface; // Surface pointer
        char msg[5];          // array for storing the value in char form
        sprintf(msg, "%d", i + 1);
        surface = TTF_RenderText_Solid(font, msg, color);               // Creating the surface
        level[i].texture = SDL_CreateTextureFromSurface(rend, surface); // Creating the texture
        SDL_FreeSurface(surface);

        count++;
        // Logic for button placements
        if (count == 5)
        {
            count = 0;
            x = 30;
            y += 60;
        }
        else
            x += 60;
    }
}

/**
 * Function to display the Random and Choose buttons
 * @param rend the SDL_Renderer used to render the window
 * @param random a MenuButton that will represent the Random option button
 * @param choose a MenuButton that will represent the Choose option button
**/
void displayMenu(SDL_Renderer *rend, MenuButton random, MenuButton choose)
{
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_RenderCopy(rend, random.texture, NULL, &(random.rect));
    SDL_RenderCopy(rend, choose.texture, NULL, &(choose.rect));
}

/**
 * Function to display the difficulty choices
 * @param rend the SDL_Renderer used to render the window
 * @param levels an array of MenuButtons to represent difficulty
**/
void displayDifficulties(SDL_Renderer *rend, MenuButton levels[20])
{
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    for (int i = 0; i < 4; i++)
    {
        SDL_RenderCopy(rend, levels[i].texture, NULL, &(levels[i].rect));
    }
}

/**
 * Function to display the level choices
 * @param rend the SDL_Renderer used to render the window
 * @param levels an array of MenuButtons to represent possible levels
**/
void displayLevels(SDL_Renderer *rend, MenuButton levels[20])
{
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    for (int i = 0; i < 20; i++)
    {
        SDL_RenderCopy(rend, levels[i].texture, NULL, &(levels[i].rect));
    }
}

/**
 * Function to check if a difficulty was selected
 * @param x the X coordinates
 * @param y the Y coordinates
 * @return Selected difficulty ID if difficulty chosen / -1 if not chosen
**/
int getDifficulty(int x, int y)
{
    // The valid y coordinates
    if (y > 30 && y < 60)
    {
        // Diff 1 X coords
        if (x > 30 && x < 60)
        {
            return 1;
        }
        // Diff 2 X coords
        if (x > 90 && x < 120)
        {
            return 2;
        }
        // Diff 3 X coords
        if (x > 150 && x < 180)
        {
            return 3;
        }
        // Diff 4 X coords
        if (x > 210 && x < 240)
        {
            return 4;
        }
    }
    return -1;
}

/**
 * Function to check if a level was clicked
 * @param x the X coordinates
 * @param y the Y coordinates
 * @param levelSelector flag to prevent auto-clicking after difficulty selection screen
set to 1 if not needed 
 * @return the selected level ID if selected / -1 if not selected
**/
int getLevel(int x, int y, int levelSelector){

    if(levelSelector == 0) // if flag not active - return
        return -1;

    int count = 0;
    
    // Initial possible coordinates
    int xCoord = 30, yCoord = 30;

    // Checking every position possible
    for (int i = 0; i < 20; i++){
        
        // if X and Y coordinates are on a button return the Button ID
        if(x > xCoord && x < xCoord + 30 && y > yCoord && y < yCoord + 30){
            return i+1;
        }
        count++;

        // Calculating valid positions
        if (count == 5)
        {
            count = 0;
             xCoord = 30;
             yCoord += 60;
        }
        else
            xCoord += 60;
    }
    return -1;
}

/**
 * Function to  clear all the textures used to render the MenuButtons
 * @param Random a MenuButton that represents the Random option button
 * @param Choose a MenuButton that represents the Choose option button
 * @param level an array containing 20 MenuButton structs that represent levels
**/
void clearTextures(MenuButton Random, MenuButton Choose, MenuButton Levels[20])
{
    // Destroying every created texture
    SDL_DestroyTexture(Random.texture);
    SDL_DestroyTexture(Choose.texture);
    for (int i = 0; i < 20; i++)
    {
        SDL_DestroyTexture(Levels[i].texture);
    }
}

/**
 * Function to display the menu to the screen and handle the difficulty and level picks
 * @param font the font used to render text on the window
 * @param rend the SDL_Renderer used to render the window
 * @param difficulty the variable to store the difficulty that has been chosen
 * @param level the variable to store the level that has been chosen / -1 if random level
**/
void generateMenu( TTF_Font *font, SDL_Renderer *rend, int* difficulty, int* level)
{
    // Menu buttons
    MenuButton randomButton;
    MenuButton chooseButton;
    MenuButton levels[20];

    // Function to create all the needed buttons
    createMenuButtons(&randomButton, &chooseButton, levels, font, rend);

    SDL_Event menuEvent;
    int displayer = 1; // variable to indicate which scene to display
    int mode = 0; // 1 for random / 2 for choose'
    int levelSelector = 0; // flag to stop level from auto-selecting
    
    // Main menu loop
    while (1)
    {
        int stopMenu = 0; // Flag to stop the loop
        while (SDL_PollEvent(&menuEvent))
        {
            if (menuEvent.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;

                // Getting the coordinates of the mouse
                SDL_GetMouseState(&x, &y);
                if (displayer == 1)
                {
                    // Checking if one of the two buttons was clicked

                    // If random button is clicked
                    if (x > 100 && x < 200 && y > 285 && y < 315)
                    {
                        //printf("clicked random\n");
                        displayer = 2;
                        mode = 1;
                    }
                    // If choose button is clicked
                    if (x > 400 && x < 500 && y > 285 && y < 315)
                    {
                        //printf("clicked choose\n");
                        mode = 2;
                        displayer = 2;
                    }
                }
                // Difficulty selection scene
                if (displayer == 2)
                {
                    // Getting the difficulty
                    *difficulty = getDifficulty(x, y);

                    // If Choose mode and difficulty selected - display 3rd scene 
                    if (mode == 2 && *difficulty > 0)
                        displayer = 3; 
                    
                    // If Random mode and difficulty selected - start game
                    else if (mode == 1 && *difficulty > 0){
                        *level = -1;
                        printf("Loading level: random, difficulty %d...\n", *difficulty);
                        stopMenu = 1;
                    }    
                }
                // Level selection scene
                if (displayer == 3)
                {
                    // Getting the level
                    *level = getLevel(x, y, levelSelector);

                    // If level chosen - start game
                    if (*level > 0)
                    {
                        printf("Loading level: %d, difficulty %d...\n",*level, *difficulty);
                        stopMenu = 1;
                    }
                    
                    // if level not chosen set flag to 1
                    else if (*level < 0)
                        levelSelector = 1;
                    
                }
            }
        }

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);

        // Making the screen black
        SDL_RenderClear(rend);
        
        // Scene display handler
        if (displayer == 1)
            displayMenu(rend, randomButton, chooseButton);
        else if (displayer == 2)
            displayDifficulties(rend, levels);
        else if (displayer == 3)
            displayLevels(rend, levels);

        SDL_RenderPresent(rend);

        if (stopMenu)
        {
            // Function to clear all the textures used to render the MenuButtons
            clearTextures(randomButton, chooseButton, levels);
            break;
        }
    }
}
