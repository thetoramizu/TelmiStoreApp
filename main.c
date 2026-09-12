#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    printf("TelmiStore SDL TEST start\n");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
    {
        printf("SDL init erreur: %s\n", SDL_GetError());
        return 1;
    }

    printf("Video driver: %s\n", SDL_GetCurrentVideoDriver());


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
        printf("Window erreur: %s\n", SDL_GetError());
        return 1;
    }

    printf("Window OK\n");


    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    printf("Window size %dx%d\n", w, h);


    SDL_Surface *surface = SDL_GetWindowSurface(window);

    if (!surface)
    {
        printf("Surface erreur: %s\n", SDL_GetError());
        return 1;
    }

    printf("Surface OK\n");


    if (SDL_NumJoysticks() > 0)
    {
        SDL_Joystick *joy = SDL_JoystickOpen(0);

        if (joy)
            printf("Joystick OK\n");
    }


    int running = 1;


    while (running)
    {
        SDL_Event e;

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = 0;


            if (e.type == SDL_KEYDOWN)
            {
                printf("Key %d\n", e.key.keysym.sym);

                // START
                if (e.key.keysym.sym == SDLK_RETURN)
                {
                    printf("Fermeture TelmiStore\n");
                    running = 0;
                }
            }
        }


        /*
            ECRAN ROUGE
        */

        SDL_FillRect(
            surface,
            NULL,
            SDL_MapRGB(
                surface->format,
                255,
                0,
                0
            )
        );


        SDL_UpdateWindowSurface(window);


        SDL_Delay(16);
    }


    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}