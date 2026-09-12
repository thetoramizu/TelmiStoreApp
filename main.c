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
        printf("Erreur fenetre: %s\n", SDL_GetError());
        return 1;
    }

    printf("Window OK\n");


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


    SDL_RendererInfo info;

    if(SDL_GetRendererInfo(renderer, &info)==0)
    {
        printf("Renderer: %s\n", info.name);
    }


    printf("Renderer OK\n");


    int joy_count = SDL_NumJoysticks();

    printf("Joysticks: %d\n", joy_count);

    if(joy_count > 0)
    {
        SDL_Joystick *joy = SDL_JoystickOpen(0);

        if(joy)
        {
            printf("Joystick OK: %s\n",
                   SDL_JoystickName(joy));
        }
    }


    int running = 1;


    while(running)
    {
        SDL_Event e;


        while(SDL_PollEvent(&e))
        {

            if(e.type == SDL_QUIT)
            {
                running = 0;
            }


            if(e.type == SDL_KEYDOWN)
            {
                printf("Key %d\n",
                       e.key.keysym.sym);


                /*
                    START = ENTER
                */
                if(e.key.keysym.sym == SDLK_RETURN)
                {
                    printf("Fermeture TelmiStore\n");
                    running = 0;
                }
            }


            if(e.type == SDL_JOYBUTTONDOWN)
            {
                printf("Joy button %d\n",
                       e.jbutton.button);
            }


            if(e.type == SDL_JOYAXISMOTION)
            {
                printf("Joy axis %d value %d\n",
                       e.jaxis.axis,
                       e.jaxis.value);
            }

        }


        /*
            TEST AFFICHAGE
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


        SDL_Delay(16);
    }



    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();


    return 0;
}