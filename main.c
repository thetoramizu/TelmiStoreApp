#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>


#define WIDTH 640
#define HEIGHT 480


FILE *logfile;


void logmsg(const char *msg)
{
    if(logfile)
    {
        fprintf(logfile, "%s\n", msg);
        fflush(logfile);
    }

    printf("%s\n", msg);
}


int main(int argc, char *argv[])
{
    logfile = fopen("telmistore.log", "w");

    logmsg("TelmiStore SDL TEST start");


    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
    {
        logmsg(SDL_GetError());
        return 1;
    }


    char buffer[256];


    snprintf(
        buffer,
        sizeof(buffer),
        "Video driver: %s",
        SDL_GetCurrentVideoDriver()
    );

    logmsg(buffer);



    SDL_Window *window = SDL_CreateWindow(
        "TelmiStore",
        0,
        0,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );


    if(!window)
    {
        logmsg("Erreur SDL_CreateWindow");
        logmsg(SDL_GetError());
        SDL_Quit();
        return 1;
    }


    logmsg("Window OK");



    SDL_Renderer *renderer =
        SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_SOFTWARE
        );


    if(!renderer)
    {
        logmsg("Erreur renderer");
        logmsg(SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    logmsg("Renderer OK");



    SDL_Joystick *joystick = NULL;


    if(SDL_NumJoysticks() > 0)
    {
        joystick = SDL_JoystickOpen(0);

        if(joystick)
            logmsg("Joystick OK");
        else
            logmsg("Erreur joystick");
    }
    else
    {
        logmsg("Pas de joystick");
    }



    int running = 1;

    Uint32 start = SDL_GetTicks();



    while(running)
    {

        SDL_Event e;


        while(SDL_PollEvent(&e))
        {

            if(e.type == SDL_QUIT)
            {
                logmsg("SDL_QUIT");
                running = 0;
            }



            if(e.type == SDL_KEYDOWN)
            {
                snprintf(
                    buffer,
                    sizeof(buffer),
                    "KEY %d",
                    e.key.keysym.sym
                );

                logmsg(buffer);


                if(e.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;
            }



            if(e.type == SDL_JOYBUTTONDOWN)
            {
                snprintf(
                    buffer,
                    sizeof(buffer),
                    "JOY BUTTON %d",
                    e.jbutton.button
                );

                logmsg(buffer);


                // B ou START selon mapping Onion
                if(e.jbutton.button == 1 ||
                   e.jbutton.button == 9)
                {
                    running = 0;
                }
            }
        }



        // sécurité : fermeture après 30 secondes
        if(SDL_GetTicks() - start > 30000)
        {
            logmsg("Timeout 30s");
            running = 0;
        }



        // fond bleu
        SDL_SetRenderDrawColor(
            renderer,
            20,
            40,
            100,
            255
        );

        SDL_RenderClear(renderer);



        // carré rouge
        SDL_Rect r1 =
        {
            100,
            100,
            200,
            100
        };


        SDL_SetRenderDrawColor(
            renderer,
            255,
            0,
            0,
            255
        );

        SDL_RenderFillRect(
            renderer,
            &r1
        );



        // carré vert
        SDL_Rect r2 =
        {
            340,
            250,
            200,
            100
        };


        SDL_SetRenderDrawColor(
            renderer,
            0,
            255,
            0,
            255
        );

        SDL_RenderFillRect(
            renderer,
            &r2
        );



        SDL_RenderPresent(renderer);


        SDL_Delay(16);
    }



    logmsg("Closing");


    if(joystick)
        SDL_JoystickClose(joystick);


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);


    SDL_Quit();


    if(logfile)
        fclose(logfile);


    return 0;
}