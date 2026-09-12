#include <SDL2/SDL.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    printf("TelmiStore SDL TEST start\n");


    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
    {
        printf("SDL Init erreur: %s\n", SDL_GetError());
        return 1;
    }


    printf("Video driver: %s\n", SDL_GetCurrentVideoDriver());


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
        SDL_Quit();
        return 1;
    }


    printf("Window OK\n");


    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );


    if (!renderer)
    {
        printf("Renderer erreur: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    printf("Renderer OK\n");


    int joycount = SDL_NumJoysticks();

    printf("Joysticks: %d\n", joycount);

    if (joycount > 0)
    {
        SDL_Joystick *joy = SDL_JoystickOpen(0);

        if (joy)
            printf("Joystick OK\n");
    }


    SDL_Color white = {255,255,255,255};

    SDL_Event e;

    int running = 1;


    while(running)
    {

        while(SDL_PollEvent(&e))
        {

            if(e.type == SDL_QUIT)
            {
                running = 0;
            }


            if(e.type == SDL_KEYDOWN)
            {
                int key = e.key.keysym.sym;

                printf("Key %d\n", key);


                switch(key)
                {

                    // B
                    case SDLK_LCTRL:
                        printf("B -> exit\n");
                        running = 0;
                        break;


                    // START
                    case SDLK_RETURN:
                        printf("START -> exit\n");
                        running = 0;
                        break;


                    case SDLK_UP:
                        printf("UP\n");
                        break;


                    case SDLK_DOWN:
                        printf("DOWN\n");
                        break;


                    case SDLK_LEFT:
                        printf("LEFT\n");
                        break;


                    case SDLK_RIGHT:
                        printf("RIGHT\n");
                        break;


                    // A
                    case SDLK_SPACE:
                        printf("A\n");
                        break;


                    // X
                    case SDLK_LSHIFT:
                        printf("X\n");
                        break;


                    // Y
                    case SDLK_LALT:
                        printf("Y\n");
                        break;


                    // SELECT
                    case SDLK_RCTRL:
                        printf("SELECT\n");
                        break;
                }
            }
        }



        /*
         * Test affichage Miyoo
         */

        SDL_SetRenderDrawColor(
            renderer,
            180,
            0,
            0,
            255
        );

        SDL_RenderClear(renderer);



        /*
         * Petit rectangle de vie
         * pour confirmer que le framebuffer bouge
         */

        SDL_Rect box;

        box.x = 220;
        box.y = 180;
        box.w = 200;
        box.h = 120;


        SDL_SetRenderDrawColor(
            renderer,
            255,
            255,
            255,
            255
        );


        SDL_RenderFillRect(
            renderer,
            &box
        );


        SDL_RenderPresent(renderer);


        SDL_Delay(16);
    }



    printf("Fermeture TelmiStore\n");


    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();


    return 0;
}