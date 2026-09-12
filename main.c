#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>


#define WIDTH 640
#define HEIGHT 480


static Uint16 framebuffer[WIDTH * HEIGHT];


void fill_screen(Uint16 color)
{
    for(int i=0;i<WIDTH*HEIGHT;i++)
        framebuffer[i]=color;
}


void draw_rect(int x,int y,int w,int h,Uint16 color)
{
    for(int yy=y; yy<y+h; yy++)
    {
        if(yy<0 || yy>=HEIGHT)
            continue;

        for(int xx=x; xx<x+w; xx++)
        {
            if(xx>=0 && xx<WIDTH)
                framebuffer[yy*WIDTH+xx]=color;
        }
    }
}


/*
    Petite police bitmap 5x7
    temporaire pour validation affichage
*/

void pixel(int x,int y,Uint16 c)
{
    if(x>=0 && x<WIDTH && y>=0 && y<HEIGHT)
        framebuffer[y*WIDTH+x]=c;
}


void text_block(int x,int y,const char *s,Uint16 color)
{
    while(*s)
    {
        for(int yy=0;yy<7;yy++)
        {
            for(int xx=0;xx<5;xx++)
            {
                if((xx+yy+(*s))%3==0)
                    pixel(x+xx,y+yy,color);
            }
        }

        x+=8;
        s++;
    }
}



int main()
{
    printf("TelmiStore MOCK start\n");


    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);


    printf("Video driver: %s\n",
        SDL_GetCurrentVideoDriver());


    SDL_Window *window =
        SDL_CreateWindow(
            "TelmiStore",
            0,
            0,
            WIDTH,
            HEIGHT,
            SDL_WINDOW_FULLSCREEN
        );


    if(!window)
    {
        printf("Window error %s\n",
            SDL_GetError());
        return 1;
    }



    SDL_Renderer *renderer =
        SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
        );


    if(!renderer)
    {
        printf("Renderer error %s\n",
            SDL_GetError());
        return 1;
    }


    SDL_Texture *texture =
        SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGB565,
            SDL_TEXTUREACCESS_STREAMING,
            WIDTH,
            HEIGHT
        );


    if(!texture)
    {
        printf("Texture error %s\n",
            SDL_GetError());
        return 1;
    }


    printf("Video pipeline OK\n");



    const char *menu[] =
    {
        "Emulateurs",
        "Favoris",
        "Parametres",
        "Quitter"
    };


    int selected=0;
    int running=1;


    while(running)
    {

        SDL_Event e;


        while(SDL_PollEvent(&e))
        {
            if(e.type==SDL_KEYDOWN)
            {
                int k=e.key.keysym.sym;

                printf("Key %d\n",k);


                if(k==SDLK_DOWN)
                {
                    selected++;
                    if(selected>3)
                        selected=0;
                }


                if(k==SDLK_UP)
                {
                    selected--;
                    if(selected<0)
                        selected=3;
                }


                if(k==SDLK_SPACE)
                {
                    printf("A select %d\n",selected);
                }


                if(k==SDLK_LCTRL)
                {
                    printf("B retour\n");
                }


                if(k==SDLK_RETURN)
                {
                    running=0;
                }
            }
        }



        /*
            Dessin
        */

        fill_screen(0x0000);


        draw_rect(
            40,
            40,
            560,
            50,
            0x07E0
        );


        text_block(
            70,
            60,
            "TELMISTORE",
            0xffff
        );


        for(int i=0;i<4;i++)
        {
            Uint16 col =
                (i==selected)
                ? 0xF800
                : 0xFFFF;


            draw_rect(
                80,
                130+i*60,
                300,
                35,
                col
            );


            text_block(
                100,
                145+i*60,
                menu[i],
                0x0000
            );
        }



        void *pixels;
        int pitch;


        if(SDL_LockTexture(
            texture,
            NULL,
            &pixels,
            &pitch)==0)
        {
            memcpy(
                pixels,
                framebuffer,
                WIDTH*HEIGHT*2
            );

            SDL_UnlockTexture(texture);
        }


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


    return 0;
}