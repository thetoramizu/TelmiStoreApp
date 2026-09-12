#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>


int main()
{
    printf("TelmiStore TEXTURE TEST start\n");


    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);


    printf("Video driver: %s\n",
        SDL_GetCurrentVideoDriver());


    SDL_Window *window = SDL_CreateWindow(
        "TelmiStore",
        0,
        0,
        640,
        480,
        SDL_WINDOW_FULLSCREEN
    );


    if (!window)
    {
        printf("Window error %s\n", SDL_GetError());
        return 1;
    }


    SDL_Renderer *renderer =
        SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
        );


    if (!renderer)
    {
        printf("Renderer error %s\n", SDL_GetError());
        return 1;
    }


    printf("Renderer OK\n");


    SDL_Texture *texture =
        SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGB565,
            SDL_TEXTUREACCESS_STREAMING,
            640,
            480
        );


    if (!texture)
    {
        printf("Texture error %s\n", SDL_GetError());
        return 1;
    }


    printf("Texture OK\n");


    void *pixels;
    int pitch;


    if (SDL_LockTexture(
            texture,
            NULL,
            &pixels,
            &pitch) == 0)
    {

        printf("Texture pitch %d\n", pitch);


        unsigned short *p = pixels;


        for(int y=0;y<480;y++)
        {
            for(int x=0;x<640;x++)
            {
                p[y*(pitch/2)+x] =
                    0xF800; // rouge RGB565
            }
        }


        SDL_UnlockTexture(texture);
    }


    SDL_RenderClear(renderer);


    SDL_RenderCopy(
        renderer,
        texture,
        NULL,
        NULL
    );


    SDL_RenderPresent(renderer);


    printf("DISPLAY DONE\n");


    while(1)
    {
        SDL_Event e;

        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_KEYDOWN)
            {
                printf("Key %d\n",
                    e.key.keysym.sym);

                if(e.key.keysym.sym == SDLK_RETURN)
                {
                    return 0;
                }
            }
        }

        SDL_Delay(16);
    }


    return 0;
}