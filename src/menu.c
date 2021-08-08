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
        sprintf(msg, "%d", i);
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

void displayMenu1(SDL_Renderer *rend, MenuButton random, MenuButton choose)
{
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_RenderCopy(rend, random.texture, NULL, &(random.rect));
    SDL_RenderCopy(rend, choose.texture, NULL, &(choose.rect));
}

void generateMenu(int matrix[9][9], TTF_Font *font, SDL_Renderer *rend)
{

    MenuButton randomButton;
    MenuButton chooseButton;
    MenuButton levels[20];
    createMenuButtons(&randomButton, &chooseButton, levels, font, rend);

    SDL_Event menuEvent;
    int displayer = 1;
    int mode = 0; // 1 for random / 2 for choose
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
                        mode = 1;
                    }
                    if (x > 400 && x < 500 && y > 285 && y < 315)
                    {
                        // Show difficulties
                        printf("clicked choose\n");
                        mode = 2;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);

        // Making the screen black
        SDL_RenderClear(rend);
        if (displayer == 1)
            displayMenu1(rend, randomButton, chooseButton);

        SDL_RenderPresent(rend);

        if (ok)
            break;
    }
}

///TO DO : clear textures