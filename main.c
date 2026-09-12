#include <SDL2/SDL.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    printf("TelmiStore SDL start\n");


    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL init erreur: %s\n", SDL_GetError());
        return 1;
    }


    SDL_Window *window = SDL_CreateWindow(
        "TelmiStore",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        0
    );


    if (!window)
    {
        printf("Erreur fenetre: %s\n", SDL_GetError());
        return 1;
    }


    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, 0);


    int running = 1;

    while(running)
    {
        SDL_Event e;

        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
                running = 0;

            if(e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;
            }
        }


        SDL_SetRenderDrawColor(
            renderer,
            0,
            0,
            0,
            255
        );

        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}