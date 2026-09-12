#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

#include "text.h"
#include "store.h"


#define W 640
#define H 480


#define BLACK  0x0000
#define WHITE  0xffff
#define RED    0xf800
#define GREEN  0x07e0
#define BLUE   0x001f
#define YELLOW 0xffe0


Uint16 framebuffer[W*H];



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



void clear_screen(Uint16 color)
{
    for(int i=0;i<W*H;i++)
        framebuffer[i]=color;
}



#define STORY_COUNT story_count



typedef enum
{
    SCREEN_LIST,
    SCREEN_DETAIL

} Screen;


static Screen screen = SCREEN_LIST;


static int selected = 0;



typedef enum
{
    DETAIL_READY,
    DETAIL_DOWNLOADING,
    DETAIL_INSTALLED

} DetailState;


static DetailState detail_state = DETAIL_READY;



void draw_list()
{
    clear_screen(BLUE);


    text_draw(
        70,
        40,
        "TELMISTORE",
        WHITE,
        3
    );


    text_draw(
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


        text_draw(
            70,
            y,
            stories[i].title,
            WHITE,
            2
        );
    }



    text_draw(
        70,
        380,
        "A DETAILS",
        YELLOW,
        2
    );


    text_draw(
        70,
        420,
        "START QUITTER",
        YELLOW,
        2
    );
}



void draw_detail()
{
    clear_screen(BLUE);


    Story *s=&stories[selected];



    text_draw(
        40,
        40,
        s->title,
        WHITE,
        2
    );


    rect(
        40,
        90,
        560,
        2,
        WHITE
    );


    text_draw_wrap(
        40,
        130,
        560,
        s->description,
        WHITE,
        2
    );



    if(detail_state==DETAIL_READY)
    {
        text_draw(
            40,
            350,
            "A TELECHARGER",
            YELLOW,
            2
        );


        text_draw(
            40,
            400,
            "B RETOUR",
            YELLOW,
            2
        );
    }



    if(detail_state==DETAIL_DOWNLOADING)
    {
        text_draw(
            40,
            350,
            "TELECHARGEMENT",
            YELLOW,
            2
        );
    }



    if(detail_state==DETAIL_INSTALLED)
    {
        text_draw(
            40,
            350,
            "INSTALLE OK",
            YELLOW,
            2
        );


        text_draw(
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
        printf("SDL ERROR %s\n", SDL_GetError());
        return 1;
    }


    printf(
        "Video driver: %s\n",
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
        printf(
            "Renderer error %s\n",
            SDL_GetError()
        );

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
        printf(
            "Texture error %s\n",
            SDL_GetError()
        );

        return 1;
    }



    /*
        Chargement du store après création écran
    */
    if(store_load()==0)
    {
        printf(
            "Impossible de charger le store\n"
        );
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


                printf(
                    "Key %d\n",
                    key
                );



                /*
                    START : quitter partout
                */
                if(key==SDLK_RETURN)
                {
                    running=0;
                }



                /*
                    Ecran liste
                */
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



                    /*
                        A : détail
                    */
                    if(key==SDLK_SPACE)
                    {
                        screen=SCREEN_DETAIL;
                        detail_state=DETAIL_READY;
                    }

                }



                /*
                    Ecran détail
                */
                else
                {

                    /*
                        B : retour
                    */
                    if(key==SDLK_LCTRL)
                    {
                        screen=SCREEN_LIST;
                    }



                    /*
                        A : téléchargement
                    */
                    if(key==SDLK_SPACE)
                    {
                        if(detail_state==DETAIL_READY)
                        {
                            detail_state=
                                DETAIL_DOWNLOADING;

                            download_start =
                                SDL_GetTicks();
                        }
                    }

                }

            }
        }



        /*
            Simulation téléchargement
            temporaire pour valider l'écran
        */
        if(detail_state==DETAIL_DOWNLOADING)
        {
            if(
                SDL_GetTicks()-download_start > 3000
            )
            {
                stories[selected].installed=1;

                detail_state=
                    DETAIL_INSTALLED;
            }
        }



        if(screen==SCREEN_LIST)
            draw_list();
        else
            draw_detail();



        /*
            Copie framebuffer -> texture
        */
        void *pixels;
        int pitch;



        if(
            SDL_LockTexture(
                tex,
                NULL,
                &pixels,
                &pitch
            )==0
        )
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