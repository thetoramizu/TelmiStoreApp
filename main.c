#include <SDL2/SDL.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    printf("TelmiStore SDL RENDER TEST start\n");


    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
    {
        printf("SDL init erreur: %s\n", SDL_GetError());
        return 1;
    }


    printf("Video driver: %s\n",
           SDL_GetCurrentVideoDriver());


    SDL_Window *window = SDL_CreateWindow(
        "TelmiStore",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_FULLSCREEN
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



    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_SOFTWARE
    );


    if (!renderer)
    {
        printf("Renderer erreur: %s\n", SDL_GetError());
        return 1;
    }


    printf("Renderer OK\n");



    if (SDL_NumJoysticks() > 0)
    {
        SDL_Joystick *joy = SDL_JoystickOpen(0);

        if (joy)
            printf("Joystick OK\n");
    }



    /*
        TEST AFFICHAGE ROUGE
    */

    SDL_SetRenderDrawColor(
        renderer,
        255,
        0,
        0,
        255
    );


    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);


    printf("RED DRAW DONE\n");



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
                printf("Key %d\n",
                       e.key.keysym.sym);


                // START
                if (e.key.keysym.sym == SDLK_RETURN)
                {
                    printf("Fermeture TelmiStore\n");
                    running = 0;
                }
            }
        }


        SDL_Delay(16);
    }



    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();


    return 0;
}