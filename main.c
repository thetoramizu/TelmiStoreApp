#include <SDL2/SDL.h>
#include <stdio.h>


int main(int argc, char **argv)
{
    FILE *log = fopen("telmistore.log","w");

    fprintf(log,"start\n");


    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(log,"SDL init error %s\n", SDL_GetError());
        return 1;
    }


    fprintf(log,"driver=%s\n",
        SDL_GetCurrentVideoDriver());


    SDL_Window *win = SDL_CreateWindow(
        "TelmiStore TEST",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_FULLSCREEN
    );


    if(!win)
    {
        fprintf(log,"window error %s\n", SDL_GetError());
        return 1;
    }


    SDL_Renderer *ren =
        SDL_CreateRenderer(
            win,
            -1,
            SDL_RENDERER_ACCELERATED
        );


    if(!ren)
    {
        fprintf(log,"renderer error %s\n", SDL_GetError());
        return 1;
    }


    fprintf(log,"renderer OK\n");



    SDL_Texture *tex =
        SDL_CreateTexture(
            ren,
            SDL_PIXELFORMAT_RGB565,
            SDL_TEXTUREACCESS_STREAMING,
            640,
            480
        );


    if(!tex)
    {
        fprintf(log,"texture error %s\n", SDL_GetError());
        return 1;
    }


    fprintf(log,"texture OK\n");



    void *pixels;
    int pitch;


    if(SDL_LockTexture(
            tex,
            NULL,
            &pixels,
            &pitch) != 0)
    {
        fprintf(log,"lock error %s\n", SDL_GetError());
        return 1;
    }



    unsigned short *p = pixels;


    // RGB565 rouge
    for(int i=0;i<640*480;i++)
    {
        p[i]=0xF800;
    }


    SDL_UnlockTexture(tex);



    SDL_RenderClear(ren);


    SDL_RenderCopy(
        ren,
        tex,
        NULL,
        NULL
    );


    SDL_RenderPresent(ren);



    fprintf(log,"present done\n");
    fflush(log);



    // attendre 10 secondes
    SDL_Delay(10000);



    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    SDL_Quit();

    fclose(log);

    return 0;
}