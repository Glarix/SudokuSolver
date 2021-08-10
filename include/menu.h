#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#ifndef __MENU__
#define __MENU__

typedef struct menuButton{
    SDL_Rect rect;
    SDL_Texture* texture;
}MenuButton;


/**
 * Function to display the menu to the screen and handle the difficulty and level picks
 * @param font the font used to render text on the window
 * @param rend the SDL_Renderer used to render the window
 * @param difficulty the variable to store the difficulty that has been chosen
 * @param level the variable to store the level that has been chosen / -1 if random level
**/
void generateMenu( TTF_Font* font, SDL_Renderer* rend, int* difficulty, int* level);

#endif