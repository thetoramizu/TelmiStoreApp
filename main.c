#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

#include "font8x8.h"
#include "store.h"

#define W 640
#define H 480


#define BLACK  0x0000
#define WHITE  0xffff
#define RED    0xf800
#define GREEN  0x07e0
#define BLUE   0x001f
#define YELLOW 0xffe0


static Uint16 framebuffer[W*H];



/*
    Données TelmiStore
*/


#define STORY_COUNT story_count


/*
    Ecrans
*/

typedef enum
{
    SCREEN_LIST,
    SCREEN_DETAIL

} Screen;


Screen screen = SCREEN_LIST;


int selected = 0;



/*
    Etat téléchargement
*/

typedef enum
{
    DETAIL_READY,
    DETAIL_DOWNLOADING,
    DETAIL_INSTALLED

} DetailState;


DetailState detail_state = DETAIL_READY;



/*
    Dessin framebuffer
*/


void clear_screen(Uint16 color)
{
    for(int i=0;i<W*H;i++)
        framebuffer[i]=color;
}



void rect(
    int x,
    int y,
    int w,
    int h,
    Uint16 color
)
{
    for(int yy=y; yy<y+h; yy++)
    {
        if(yy<0 || yy>=H)
            continue;


        for(int xx=x; xx<x+w; xx++)
        {
            if(xx>=0 && xx<W)
                framebuffer[yy*W+xx]=color;
        }
    }
}



/*
    Police bitmap
*/


const uint8_t *glyph(char c)
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



void draw_char(
    int x,
    int y,
    char c,
    Uint16 color,
    int scale
)
{
    const uint8_t *g=glyph(c);


if(!g)
{
    // carré de remplacement pour caractère absent
    rect(x,y,8*scale,8*scale,RED);
    return;
}


    for(int row=0;row<8;row++)
    {
        for(int col=0;col<8;col++)
        {
            if(g[row] & (0x80>>col))
            {
                rect(
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



void draw_text(
    int x,
    int y,
    const char *txt,
    Uint16 color,
    int scale
)
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


        x += (8*scale)+2;

        txt++;
    }
}
void draw_list()
{
    clear_screen(BLUE);


    draw_text(
        70,
        40,
        "TELMISTORE",
        WHITE,
        3
    );


    draw_text(
        70,
        100,
        "STORIES",
        YELLOW,
        2
    );


    for(int i=0;i<STORY_COUNT;i++)
    {
        int y = 160 + i*55;


        if(i==selected)
        {
            rect(
                50,
                y-5,
                540,
                40,
                RED
            );
        }


        draw_text(
            70,
            y,
            stories[i].title,
            WHITE,
            2
        );
    }


    draw_text(
        70,
        380,
        "A DETAILS",
        YELLOW,
        2
    );


    draw_text(
        70,
        420,
        "B QUITTER",
        YELLOW,
        2
    );
}



void draw_detail()
{
    clear_screen(BLUE);


    Story *s=&stories[selected];


    draw_text(
        40,
        40,
        s->title,
        WHITE,
        3
    );


    rect(
        40,
        90,
        560,
        2,
        WHITE
    );


    draw_text(
        40,
        130,
        s->description,
        WHITE,
        2
    );



    if(detail_state==DETAIL_READY)
    {
        draw_text(
            40,
            350,
            "A TELECHARGER",
            YELLOW,
            2
        );

        draw_text(
            40,
            400,
            "B RETOUR",
            YELLOW,
            2
        );
    }


    if(detail_state==DETAIL_DOWNLOADING)
    {
        draw_text(
            40,
            350,
            "TELECHARGEMENT...",
            YELLOW,
            2
        );
    }


    if(detail_state==DETAIL_INSTALLED)
    {
        draw_text(
            40,
            350,
            "INSTALLE OK",
            YELLOW,
            2
        );

        draw_text(
            40,
            400,
            "B RETOUR",
            YELLOW,
            2
        );
    }
}




int main()
{
    printf("TelmiStore STORIES start\n");


    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK)!=0)
    {
        printf("SDL ERROR %s\n",SDL_GetError());
        return 1;
    }

    if(store_load()==0)
    {
        printf("Impossible de charger le store\n");
    }

    printf("Video driver: %s\n",
        SDL_GetCurrentVideoDriver()
    );



    SDL_Window *win =
        SDL_CreateWindow(
            "TelmiStore",
            0,
            0,
            W,
            H,
            SDL_WINDOW_FULLSCREEN
        );


    if(!win)
    {
        printf("Window error\n");
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
        printf("Renderer error %s\n",
            SDL_GetError());
        return 1;
    }



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
        printf("Texture error\n");
        return 1;
    }



    int running=1;


    Uint32 download_start=0;



    while(running)
    {
        SDL_Event e;


        while(SDL_PollEvent(&e))
        {
            if(e.type==SDL_KEYDOWN)
            {
                int key=e.key.keysym.sym;


                printf("Key %d\n",key);



                // START
                if(key==SDLK_RETURN)
                {
                    running=0;
                }



                // LISTE
                if(screen==SCREEN_LIST)
                {

                    if(key==SDLK_DOWN)
                    {
                        selected++;

                        if(selected>=STORY_COUNT)
                            selected=0;
                    }


                    if(key==SDLK_UP)
                    {
                        selected--;

                        if(selected<0)
                            selected=STORY_COUNT-1;
                    }



                    // A
                    if(key==SDLK_SPACE)
                    {
                        screen=SCREEN_DETAIL;
                        detail_state=DETAIL_READY;
                    }
                }



                // DETAIL
                else
                {

                    // B
                    if(key==SDLK_LCTRL)
                    {
                        screen=SCREEN_LIST;
                    }



                    // A
                    if(key==SDLK_SPACE)
                    {

                        if(detail_state==DETAIL_READY)
                        {
                            detail_state=DETAIL_DOWNLOADING;
                            download_start=SDL_GetTicks();
                        }

                    }

                }

            }
        }




        if(detail_state==DETAIL_DOWNLOADING)
        {
            if(SDL_GetTicks()-download_start > 3000)
            {
                stories[selected].installed=1;
                detail_state=DETAIL_INSTALLED;
            }
        }




        if(screen==SCREEN_LIST)
            draw_list();
        else
            draw_detail();





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