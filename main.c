#include <SDL2/SDL.h>
#include <stdio.h>


#define WIDTH 640
#define HEIGHT 480


int main(int argc, char *argv[])
{
    printf("TelmiStore SDL TEST start\n");


    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
    {
        printf("SDL init erreur: %s\n", SDL_GetError());
        return 1;
    }


    SDL_Window *window = SDL_CreateWindow(
        "TelmiStore TEST",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        0
    );


    if(!window)
    {
        printf("Erreur fenetre: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }


    SDL_Renderer *renderer =
        SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_SOFTWARE
        );


    if(!renderer)
    {
        printf("Erreur renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    SDL_Joystick *joystick = NULL;


    if(SDL_NumJoysticks() > 0)
    {
        joystick = SDL_JoystickOpen(0);

        if(joystick)
            printf("Joystick OK\n");
        else
            printf("Erreur joystick: %s\n", SDL_GetError());
    }
    else
    {
        printf("Pas de joystick\n");
    }


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
                printf("Key %d\n", e.key.keysym.sym);

                if(e.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;
            }


            if(e.type == SDL_JOYBUTTONDOWN)
            {
                printf("Joy button %d\n", e.jbutton.button);

                // B
                if(e.jbutton.button == 1)
                    running = 0;
            }
        }


        // fond bleu foncé
        SDL_SetRenderDrawColor(
            renderer,
            20,
            40,
            80,
            255
        );

        SDL_RenderClear(renderer);


        // rectangle rouge
        SDL_Rect r1 =
        {
            100,
            100,
            200,
            100
        };


        SDL_SetRenderDrawColor(
            renderer,
            255,
            0,
            0,
            255
        );

        SDL_RenderFillRect(
            renderer,
            &r1
        );


        // rectangle vert
        SDL_Rect r2 =
        {
            340,
            250,
            200,
            100
        };


        SDL_SetRenderDrawColor(
            renderer,
            0,
            255,
            0,
            255
        );

        SDL_RenderFillRect(
            renderer,
            &r2
        );


        SDL_RenderPresent(renderer);


        SDL_Delay(16);
    }


    printf("Fermeture TelmiStore\n");


    if(joystick)
        SDL_JoystickClose(joystick);


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();


    return 0;
}