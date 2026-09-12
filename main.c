#include <SDL2/SDL.h>
#include <stdio.h>


#define WIDTH 640
#define HEIGHT 480


typedef struct
{
    const char *title;
    const char *zip;
} Story;


Story stories[] =
{
    {"La grande aventure spatiale", "giglaxe.zip"},
    {"Voyage en terre ocre", "mpa.zip"}
};


int main(int argc, char *argv[])
{
    printf("TelmiStore SDL start\n");


    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
    {
        printf("SDL init erreur: %s\n", SDL_GetError());
        return 1;
    }


    SDL_Window *window = SDL_CreateWindow(
        "TelmiStore",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        0
    );


    if (!window)
    {
        printf("Fenetre erreur: %s\n", SDL_GetError());
        return 1;
    }


    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, 0);


    if (!renderer)
    {
        printf("Renderer erreur: %s\n", SDL_GetError());
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


    int selected = 0;
    int running = 1;


    while(running)
    {
        SDL_Event e;


        while(SDL_PollEvent(&e))
        {

            if(e.type == SDL_QUIT)
                running = 0;


            if(e.type == SDL_JOYBUTTONDOWN)
            {
                printf("Bouton %d\n", e.jbutton.button);


                // Haut
                if(e.jbutton.button == 4)
                {
                    selected--;
                    if(selected < 0)
                        selected = 1;
                }


                // Bas
                if(e.jbutton.button == 5)
                {
                    selected++;
                    if(selected > 1)
                        selected = 0;
                }


                // A
                if(e.jbutton.button == 0)
                {
                    printf(
                        "Selection : %s\n",
                        stories[selected].title
                    );
                }


                // B
                if(e.jbutton.button == 1)
                {
                    running = 0;
                }
            }
        }


        // fond
        SDL_SetRenderDrawColor(
            renderer,
            20,
            20,
            30,
            255
        );

        SDL_RenderClear(renderer);


        // cases histoires
        for(int i=0;i<2;i++)
        {

            SDL_Rect box;

            box.x = 50;
            box.y = 80 + i*100;
            box.w = 540;
            box.h = 70;


            if(i == selected)
            {
                SDL_SetRenderDrawColor(
                    renderer,
                    80,
                    80,
                    200,
                    255
                );
            }
            else
            {
                SDL_SetRenderDrawColor(
                    renderer,
                    50,
                    50,
                    60,
                    255
                );
            }


            SDL_RenderFillRect(
                renderer,
                &box
            );
        }


        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }


if(joystick)
{
    SDL_JoystickClose(joystick);
}

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();


    return 0;
}