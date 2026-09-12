#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    printf("TelmiStore TEST start\n");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
    {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    printf("Video driver: %s\n", SDL_GetCurrentVideoDriver());


    SDL_Window *win = SDL_CreateWindow(
        "TelmiStore",
        0,
        0,
        640,
        480,
        SDL_WINDOW_FULLSCREEN
    );

    if (!win)
    {
        printf("Window error: %s\n", SDL_GetError());
        return 1;
    }

    printf("Window OK\n");


    SDL_Renderer *ren = SDL_CreateRenderer(
        win,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!ren)
    {
        printf("Renderer error: %s\n", SDL_GetError());
        return 1;
    }

    printf("Renderer OK\n");


    SDL_Texture *tex = SDL_CreateTexture(
        ren,
        SDL_PIXELFORMAT_RGB565,
        SDL_TEXTUREACCESS_STREAMING,
        640,
        480
    );

    if (!tex)
    {
        printf("Texture error: %s\n", SDL_GetError());
        return 1;
    }

    printf("Texture OK\n");


    /* écran rouge RGB565 */
    void *pixels;
    int pitch;

    if (SDL_LockTexture(tex, NULL, &pixels, &pitch) == 0)
    {
        unsigned short *p = (unsigned short *)pixels;

        for (int y = 0; y < 480; y++)
        {
            for (int x = 0; x < 640; x++)
            {
                /*
                 * RGB565:
                 * rouge = 11111 000000 00000
                 */
                p[y * (pitch / 2) + x] = 0xF800;
            }
        }

        SDL_UnlockTexture(tex);
    }


    SDL_Joystick *joy = NULL;

    if (SDL_NumJoysticks() > 0)
    {
        joy = SDL_JoystickOpen(0);
        printf("Joystick OK\n");
    }


    SDL_Event e;
    int running = 1;


    while (running)
    {

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = 0;


            if (e.type == SDL_KEYDOWN)
            {
                printf("Key %d\n", e.key.keysym.sym);

                /*
                 * START / ENTER / ESC quittent
                 */
                if (e.key.keysym.sym == SDLK_RETURN ||
                    e.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = 0;
                }
            }

            if (e.type == SDL_JOYBUTTONDOWN)
            {
                printf("Joy button %d\n", e.jbutton.button);

                /*
                 * bouton B souvent = 1
                 */
                if (e.jbutton.button == 1)
                    running = 0;
            }
        }


        SDL_RenderClear(ren);

        SDL_RenderCopy(
            ren,
            tex,
            NULL,
            NULL
        );

        SDL_RenderPresent(ren);


        SDL_Delay(16);
    }


    printf("Exit TelmiStore\n");

    if (joy)
        SDL_JoystickClose(joy);

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}