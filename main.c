#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "font8x8.h"

#define W 640
#define H 480

#define BLACK 0x0000
#define WHITE 0xffff
#define RED   0xf800
#define GREEN 0x07e0
#define BLUE  0x001f
#define PURPLE 0x780f
#define YELLOW 0xffe0


static Uint16 framebuffer[W*H];


void clear_screen(Uint16 c)
{
    for(int i=0;i<W*H;i++)
        framebuffer[i]=c;
}


void draw_rect(int x,int y,int w,int h,Uint16 c)
{
    for(int yy=y; yy<y+h; yy++)
    {
        if(yy<0 || yy>=H)
            continue;

        for(int xx=x; xx<x+w; xx++)
        {
            if(xx>=0 && xx<W)
                framebuffer[yy*W+xx]=c;
        }
    }
}


const uint8_t *find_glyph(char c)
{
    if(c>='a' && c<='z')
        c-=32;

    for(unsigned int i=0;i<FONT8X8_COUNT;i++)
    {
        if(font8x8[i].c==c)
            return font8x8[i].data;
    }

    return NULL;
}



void draw_char(int x,int y,char c,Uint16 color,int scale)
{
    const uint8_t *glyph=find_glyph(c);

    if(!glyph)
        return;


    for(int row=0;row<8;row++)
    {
        for(int col=0;col<8;col++)
        {
            if(glyph[row] & (1<<col))
            {
                draw_rect(
                    x+col*scale,
                    y+row*scale,
                    scale,
                    scale,
                    color
                );
            }
        }
    }
}



void draw_text(int x,int y,const char *txt,Uint16 color,int scale)
{
    while(*txt)
    {
        draw_char(
            x,
            y,
            *txt,
            color,
            scale
        );

        x+=8*scale+2;
        txt++;
    }
}



const char *stories[] =
{
    "L ETE DE CECILE",
    "LA FORET MAGIQUE",
    "NOEL CHEZ LEON",
    "L ILE MYSTERIEUSE"
};


int main()
{

    printf("TelmiStore TEXT TEST\n");


    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);


    SDL_Window *win =
        SDL_CreateWindow(
            "TelmiStore",
            0,
            0,
            W,
            H,
            SDL_WINDOW_FULLSCREEN
        );


    SDL_Renderer *ren =
        SDL_CreateRenderer(
            win,
            -1,
            SDL_RENDERER_ACCELERATED
        );


    SDL_Texture *tex =
        SDL_CreateTexture(
            ren,
            SDL_PIXELFORMAT_RGB565,
            SDL_TEXTUREACCESS_STREAMING,
            W,
            H
        );


    if(!tex)
    {
        printf("texture error\n");
        return 1;
    }


    int running=1;
    int selected=0;
    int screen=0;


    while(running)
    {

        SDL_Event e;


        while(SDL_PollEvent(&e))
        {

            if(e.type==SDL_KEYDOWN)
            {

                int k=e.key.keysym.sym;

                printf("Key %d\n",k);


                if(k==SDLK_RETURN)
                    running=0;


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


                // A
                if(k==SDLK_SPACE)
                    screen=1;


                // B
                if(k==SDLK_LCTRL)
                    screen=0;
            }
        }



        if(screen==0)
        {
            clear_screen(GREEN);

            draw_text(
                80,
                70,
                "TELMISTORE",
                BLACK,
                3
            );


            draw_text(
                80,
                180,
                "A STORIES",
                WHITE,
                2
            );
        }
        else
        {

            clear_screen(BLUE);


            draw_text(
                60,
                40,
                "HISTOIRES",
                WHITE,
                3
            );


            for(int i=0;i<4;i++)
            {

                if(i==selected)
                    draw_rect(
                        50,
                        120+i*50,
                        520,
                        35,
                        RED
                    );


                draw_text(
                    70,
                    130+i*50,
                    stories[i],
                    WHITE,
                    2
                );

            }


            draw_text(
                50,
                390,
                "START QUIT",
                YELLOW,
                2
            );
        }



        void *pixels;
        int pitch;


        if(SDL_LockTexture(
            tex,
            NULL,
            &pixels,
            &pitch)==0)
        {

            memcpy(
                pixels,
                framebuffer,
                W*H*2
            );

            SDL_UnlockTexture(tex);
        }



        SDL_RenderCopy(
            ren,
            tex,
            NULL,
            NULL
        );


        SDL_RenderPresent(ren);


        SDL_Delay(16);
    }


    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    SDL_Quit();


    return 0;
}