#include <SDL2/SDL.h>
#include <stdio.h>


int main()
{
    FILE *log=fopen("telmistore.log","w");

    fprintf(log,"START\n");

    SDL_Init(SDL_INIT_VIDEO);


    fprintf(log,"driver=%s\n",
        SDL_GetCurrentVideoDriver());


    SDL_Window *win = SDL_CreateWindow(
        "TEST",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_FULLSCREEN
    );


    if(!win)
    {
        fprintf(log,"window error %s\n",SDL_GetError());
        return 1;
    }


    fprintf(log,"window OK\n");


    SDL_Renderer *ren =
        SDL_CreateRenderer(
            win,
            -1,
            SDL_RENDERER_ACCELERATED
        );


    if(!ren)
    {
        fprintf(log,"renderer error %s\n",SDL_GetError());
        return 1;
    }


    SDL_RendererInfo info;
    SDL_GetRendererInfo(ren,&info);

    fprintf(log,"renderer=%s\n",
        info.name);


    int running=1;


    while(running)
    {
        SDL_Event e;

        while(SDL_PollEvent(&e))
        {
            if(e.type==SDL_KEYDOWN)
            {
                fprintf(log,"KEY %d\n",
                    e.key.keysym.sym);
                fflush(log);

                if(e.key.keysym.sym==SDLK_ESCAPE)
                    running=0;
            }
        }


        SDL_SetRenderDrawColor(
            ren,
            255,
            0,
            0,
            255
        );


        SDL_RenderClear(ren);


        SDL_RenderPresent(ren);


        SDL_Delay(16);
    }


    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    SDL_Quit();

    fclose(log);

    return 0;
}