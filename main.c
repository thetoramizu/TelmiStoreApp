#include <SDL2/SDL.h>
#include <stdio.h>


#define WIDTH 640
#define HEIGHT 480


FILE *logfile;


void logmsg(const char *msg)
{
    printf("%s\n", msg);

    if(logfile)
    {
        fprintf(logfile, "%s\n", msg);
        fflush(logfile);
    }
}


int main(int argc, char *argv[])
{
    logfile = fopen("telmistore.log", "w");


    logmsg("TelmiStore SDL TEST start");


    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
    {
        logmsg("SDL Init erreur");
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
        logmsg("Erreur creation fenetre");
        logmsg(SDL_GetError());
        return 1;
    }


    logmsg("Window OK");



    int w,h;

    SDL_GetWindowSize(
        window,
        &w,
        &h
    );


    snprintf(
        buffer,
        sizeof(buffer),
        "Window size %dx%d",
        w,
        h
    );

    logmsg(buffer);



    SDL_Surface *screen =
        SDL_GetWindowSurface(window);


    if(!screen)
    {
        logmsg("Erreur surface");
        logmsg(SDL_GetError());
        return 1;
    }


    logmsg("Surface OK");



    SDL_Joystick *joystick = NULL;


    if(SDL_NumJoysticks() > 0)
    {
        joystick = SDL_JoystickOpen(0);

        if(joystick)
            logmsg("Joystick OK");
        else
            logmsg("Joystick erreur");
    }



    int running = 1;


    Uint32 start =
        SDL_GetTicks();



    while(running)
    {

        SDL_Event e;


        while(SDL_PollEvent(&e))
        {

            if(e.type == SDL_QUIT)
                running = 0;



            if(e.type == SDL_KEYDOWN)
            {
                snprintf(
                    buffer,
                    sizeof(buffer),
                    "KEY %d",
                    e.key.keysym.sym
                );

                logmsg(buffer);


                // ESC
                if(e.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;


                // B Miyoo souvent espace
                if(e.key.keysym.sym == SDLK_SPACE)
                    running = 0;
            }
        }



        /*
            Fond blanc
        */

        SDL_FillRect(
            screen,
            NULL,
            SDL_MapRGB(
                screen->format,
                255,
                255,
                255
            )
        );



        /*
            Rectangle rouge
        */

        SDL_Rect red =
        {
            100,
            100,
            200,
            100
        };


        SDL_FillRect(
            screen,
            &red,
            SDL_MapRGB(
                screen->format,
                255,
                0,
                0
            )
        );



        /*
            Rectangle bleu
        */

        SDL_Rect blue =
        {
            350,
            250,
            150,
            100
        };


        SDL_FillRect(
            screen,
            &blue,
            SDL_MapRGB(
                screen->format,
                0,
                0,
                255
            )
        );



        SDL_UpdateWindowSurface(
            window
        );



        /*
            Sécurité : fermeture après 30 secondes
        */

        if(SDL_GetTicks() - start > 30000)
        {
            logmsg("Timeout");
            running = 0;
        }


        SDL_Delay(16);
    }



    logmsg("Fermeture TelmiStore");



    if(joystick)
        SDL_JoystickClose(joystick);



    SDL_DestroyWindow(
        window
    );


    SDL_Quit();



    if(logfile)
        fclose(logfile);


    return 0;
}