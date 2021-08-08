#include "menu.h"

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

void displayMenu(SDL_Renderer *rend, MenuButton random, MenuButton choose)
{
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_RenderCopy(rend, random.texture, NULL, &(random.rect));
    SDL_RenderCopy(rend, choose.texture, NULL, &(choose.rect));
}

void displayDifficulties(SDL_Renderer *rend, MenuButton levels[20])
{
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    for (int i = 0; i < 4; i++)
    {
        SDL_RenderCopy(rend, levels[i].texture, NULL, &(levels[i].rect));
    }
}

void displayLevels(SDL_Renderer *rend, MenuButton levels[20])
{
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    for (int i = 0; i < 20; i++)
    {
        SDL_RenderCopy(rend, levels[i].texture, NULL, &(levels[i].rect));
    }
}

int getDifficulty(int x, int y)
{

    if (y > 30 && y < 60)
    {
        if (x > 30 && x < 60)
        {
            printf("Clicked: 1\n");
            return 1;
        }
        if (x > 90 && x < 120)
        {
            printf("Clicked: 2\n");
            return 2;
        }

        if (x > 150 && x < 180)
        {
            printf("Clicked: 3\n");
            return 3;
        }
        if (x > 210 && x < 240)
        {
            printf("Clicked: 4\n");
            return 4;
        }
    }
    return -1;
}

int getLevel(int x, int y, int levelSelector){

    if(levelSelector == 0)
        return -1;

    int count = 0;
    int xCoord = 30, yCoord = 30;
    for (int i = 0; i < 20; i++){

        if(x > xCoord && x < xCoord + 30 && y > yCoord && y < yCoord + 30){
            return i+1;
        }
        count++;

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

void clearTextures(MenuButton Random, MenuButton Choose, MenuButton Levels[20])
{
    SDL_DestroyTexture(Random.texture);
    SDL_DestroyTexture(Choose.texture);
    for (int i = 0; i < 20; i++)
    {
        SDL_DestroyTexture(Levels[i].texture);
    }
}

void generateMenu(int matrix[9][9], TTF_Font *font, SDL_Renderer *rend, int* difficulty, int* level)
{

    MenuButton randomButton;
    MenuButton chooseButton;
    MenuButton levels[20];
    createMenuButtons(&randomButton, &chooseButton, levels, font, rend);

    SDL_Event menuEvent;
    int displayer = 1;
    int mode = 0; // 1 for random / 2 for choose'
    int levelSelector = 0; // flag to stop level from auto-selecting
    while (1)
    {
        int ok = 0;
        while (SDL_PollEvent(&menuEvent))
        {
            if (menuEvent.type == SDL_KEYDOWN)
            {
                ok = 1;
                break;
            }
            else if (menuEvent.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (displayer == 1)
                {

                    if (x > 100 && x < 200 && y > 285 && y < 315)
                    {
                        printf("clicked random\n");
                        displayer = 2;
                        mode = 1;
                    }
                    if (x > 400 && x < 500 && y > 285 && y < 315)
                    {
                        printf("clicked choose\n");
                        mode = 2;
                        displayer = 2;
                    }
                }
                if (displayer == 2)
                {
                    *difficulty = getDifficulty(x, y);
                    if (mode == 2 && *difficulty > 0)
                        displayer = 3; 
                    else if (mode == 1 && *difficulty > 0){
                        *level = -1;
                        ok = 1;
                    }    
                }
                if (displayer == 3)
                {
                    *level = getLevel(x, y, levelSelector);
                    if (*level > 0)
                    {
                        printf("Dif: %d\nLev: %d\n", *difficulty, *level);
                        ok = 1;
                    }
                    else if (*level < 0)
                        levelSelector = 1;
                    
                }
            }
        }

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);

        // Making the screen black
        SDL_RenderClear(rend);
        if (displayer == 1)
            displayMenu(rend, randomButton, chooseButton);
        else if (displayer == 2)
            displayDifficulties(rend, levels);
        else if (displayer == 3)
            displayLevels(rend, levels);

        SDL_RenderPresent(rend);

        if (ok)
        {
            clearTextures(randomButton, chooseButton, levels);
            break;
        }
    }
}
