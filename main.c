#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

#define W 640
#define H 480


static Uint16 fb[W*H];


/* RGB565 */
#define BLACK 0x0000
#define WHITE 0xffff
#define RED   0xf800
#define GREEN 0x07e0
#define BLUE  0x001f
#define YELLOW 0xffe0


void clear(Uint16 c)
{
    for(int i=0;i<W*H;i++)
        fb[i]=c;
}


void rect(int x,int y,int w,int h,Uint16 c)
{
    for(int yy=y; yy<y+h; yy++)
    {
        if(yy<0 || yy>=H)
            continue;

        for(int xx=x; xx<x+w; xx++)
        {
            if(xx>=0 && xx<W)
                fb[yy*W+xx]=c;
        }
    }
}


/*
 Petite police de test.
 Chaque caractère est volontairement simple :
 on valide d'abord le rendu.
*/

void draw_char(int x,int y,char c,Uint16 col)
{
    unsigned int seed=(unsigned int)c;

    for(int yy=0;yy<12;yy++)
    {
        for(int xx=0;xx<8;xx++)
        {
            if(((seed>>(xx%5)) ^ yy) & 1)
                fb[(y+yy)*W+x+xx]=col;
        }
    }
}


void text(int x,int y,const char *s,Uint16 col)
{
    while(*s)
    {
        draw_char(x,y,*s,col);
        x+=10;
        s++;
    }
}



typedef struct
{
    const char *title;
    const char *content;
} Story;


Story stories[] =
{
    {
        "Le petit prince",
        "Une histoire dans les etoiles..."
    },
    {
        "Le dragon bleu",
        "Un dragon protege une foret magique."
    },
    {
        "Voyage spatial",
        "Une aventure parmi les planetes."
    },
    {
        "La foret",
        "Un mystere cache entre les arbres."
    }
};


int main()
{
    FILE *log=fopen("telmistore.log","a");

    printf("TelmiStore Stories start\n");

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);


    fprintf(log,"Video driver=%s\n",
        SDL_GetCurrentVideoDriver());


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
        printf("Texture error %s\n",
            SDL_GetError());
        return 1;
    }


    fprintf(log,"Texture OK\n");
    fflush(log);



    enum {
        HOME,
        STORIES,
        READER
    } screen=HOME;


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


                if(k==SDLK_RETURN)
                {
                    running=0;
                }


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


                /* A */
                if(k==SDLK_SPACE)
                {
                    if(screen==HOME)
                        screen=STORIES;
                    else if(screen==STORIES)
                        screen=READER;
                }


                /* B */
                if(k==SDLK_LCTRL)
                {
                    if(screen==READER)
                        screen=STORIES;
                    else if(screen==STORIES)
                        screen=HOME;
                }

            }
        }



        clear(BLACK);



        if(screen==HOME)
        {

            rect(40,40,560,60,GREEN);

            text(
                80,
                65,
                "TELMISTORE",
                BLACK
            );


            text(
                80,
                150,
                "> STORIES",
                WHITE
            );


            text(
                80,
                210,
                "A OUVRIR",
                YELLOW
            );


        }



        if(screen==STORIES)
        {

            text(
                50,
                40,
                "HISTOIRES",
                WHITE
            );


            for(int i=0;i<4;i++)
            {
                Uint16 c =
                    (i==selected)
                    ? RED
                    : BLUE;


                rect(
                    60,
                    100+i*60,
                    420,
                    35,
                    c
                );


                text(
                    80,
                    110+i*60,
                    stories[i].title,
                    WHITE
                );
            }

        }



        if(screen==READER)
        {

            text(
                50,
                40,
                stories[selected].title,
                GREEN
            );


            text(
                50,
                140,
                stories[selected].content,
                WHITE
            );


            text(
                50,
                350,
                "B RETOUR",
                YELLOW
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
                fb,
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



    fprintf(log,"exit\n");

    fclose(log);


    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}