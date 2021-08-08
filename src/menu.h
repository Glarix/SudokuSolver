#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#ifndef __MENU__
#define __MENU__

typedef struct menuButton{
    SDL_Rect rect;
    SDL_Texture* texture;
}MenuButton;

void generateMenu(int matrix[9][9], TTF_Font* font, SDL_Renderer* rend);

#endif