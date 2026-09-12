#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>


#define WIDTH 640
#define HEIGHT 480


static SDL_Renderer *renderer;
static SDL_Texture *texture;


/*
    RGB565 helpers
*/
static unsigned short rgb565(
    unsigned char r,
    unsigned char g,
    unsigned char b)
{
    return ((r >> 3) << 11) |
           ((g >> 2) << 5) |
           (b >> 3);
}


/*
    Dessine un rectangle dans le framebuffer
*/
static void rect(
    unsigned short *fb,
    int x,
    int y,
    int w,
    int h,
    unsigned short color)
{
    for(int yy=y; yy<y+h; yy++)
    {
        if(yy<0 || yy>=HEIGHT)
            continue;

        for(int xx=x; xx<x+w; xx++)
        {
            if(xx<0 || xx>=WIDTH)
                continue;

            fb[yy*WIDTH+xx]=color;
        }
    }
}


/*
    Petite police bitmap ultra simple
    (juste des blocs pour valider l'affichage)
*/
static void draw_box_text(
    unsigned short *fb,
    int x,
    int y,
    int len,
    unsigned short color)
{
    for(int i=0;i<len;i++)
    {
        rect(
            fb,
            x+i*12,
            y,
            8,
            12,
            color
        );
    }
}



int main()
{
    FILE *log=fopen("telmistore.log","w");


    fprintf(log,"TelmiStore start\n");


    if(SDL_Init(SDL_INIT_VIDEO)!=0)
    {
        fprintf(log,"SDL init error %s\n",
            SDL_GetError());
        return 1;
    }


    fprintf(log,"driver=%s\n",
        SDL_GetCurrentVideoDriver());



    SDL_Window *window =
        SDL_CreateWindow(
            "TelmiStore",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WIDTH,
            HEIGHT,
            SDL_WINDOW_FULLSCREEN
        );


    if(!window)
    {
        fprintf(log,"window error %s\n",
            SDL_GetError());
        return 1;
    }



    renderer =
        SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
        );


    if(!renderer)
    {
        fprintf(log,"renderer error %s\n",
            SDL_GetError());
        return 1;
    }



    texture =
        SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGB565,
            SDL_TEXTUREACCESS_STREAMING,
            WIDTH,
            HEIGHT
        );


    if(!texture)
    {
        fprintf(log,"texture error %s\n",
            SDL_GetError());
        return 1;
    }


    fprintf(log,"graphics OK\n");



    int selected=0;

    const char *stories[] =
    {
        "Story One",
        "Story Two",
        "Story Three"
    };


    int running=1;


    while(running)
    {
        SDL_Event e;


        while(SDL_PollEvent(&e))
        {
            if(e.type==SDL_KEYDOWN)
            {
                SDL_Keycode k=e.key.keysym.sym;


                fprintf(log,"key %d\n",k);
                fflush(log);


                if(k==SDLK_UP)
                {
                    selected--;
                    if(selected<0)
                        selected=2;
                }


                if(k==SDLK_DOWN)
                {
                    selected++;
                    if(selected>2)
                        selected=0;
                }


                if(k==SDLK_SPACE)
                {
                    fprintf(log,
                        "Selected %s\n",
                        stories[selected]);
                }


                if(k==SDLK_RETURN ||
                   k==SDLK_ESCAPE)
                {
                    running=0;
                }
            }
        }



        void *pixels;
        int pitch;


        if(SDL_LockTexture(
            texture,
            NULL,
            &pixels,
            &pitch)==0)
        {

            unsigned short *fb=pixels;


            /*
                fond noir
            */
            for(int i=0;i<WIDTH*HEIGHT;i++)
                fb[i]=rgb565(0,0,0);



            /*
                titre
            */
            draw_box_text(
                fb,
                40,
                40,
                10,
                rgb565(255,255,255)
            );


            /*
                menu
            */
            for(int i=0;i<3;i++)
            {
                unsigned short c;

                if(i==selected)
                    c=rgb565(0,200,255);
                else
                    c=rgb565(100,100,100);


                rect(
                    fb,
                    80,
                    120+i*70,
                    300,
                    40,
                    c
                );
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


        SDL_Delay(16);
    }



    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();


    fclose(log);

    return 0;
}