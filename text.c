#include "text.h"
#include "font8x8.h"

#include <string.h>


/*
    Fourni par main.c
    Le moteur texte ne connait pas le framebuffer.
*/
extern void rect(
    int x,
    int y,
    int w,
    int h,
    uint16_t color
);



static const uint8_t *get_glyph(char c)
{
    /*
        Pour l'instant la police est en majuscules.
        On convertit donc les minuscules.
    */
    if(c >= 'a' && c <= 'z')
        c -= 32;


    for(unsigned int i = 0; i < FONT8X8_COUNT; i++)
    {
        if(font8x8[i].c == c)
            return font8x8[i].data;
    }


    return NULL;
}



static void draw_char(
    int x,
    int y,
    char c,
    uint16_t color,
    int scale
)
{
    const uint8_t *glyph = get_glyph(c);


    /*
        Caractère inconnu :
        carré rouge de debug
    */
    if(!glyph)
    {
        rect(
            x,
            y,
            8 * scale,
            8 * scale,
            0xf800
        );

        return;
    }



    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            if(glyph[row] & (0x80 >> col))
            {
                rect(
                    x + col * scale,
                    y + row * scale,
                    scale,
                    scale,
                    color
                );
            }
        }
    }
}



int text_width(
    const char *txt,
    int scale
)
{
    int width = 0;


    while(*txt)
    {
        width += (8 * scale) + 2;
        txt++;
    }


    return width;
}



void text_draw(
    int x,
    int y,
    const char *txt,
    uint16_t color,
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


        x += (8 * scale) + 2;

        txt++;
    }
}



void text_draw_wrap(
    int x,
    int y,
    int width,
    const char *txt,
    uint16_t color,
    int scale
)
{
    int start_x = x;

    int line_height =
        (8 * scale) + 6;



    char word[128];


    while(*txt)
    {
        int len = 0;


        /*
            Extraction d'un mot
        */
        while(
            txt[len] &&
            txt[len] != ' '
        )
        {
            len++;
        }


        if(len >= (int)sizeof(word))
            len = sizeof(word)-1;


        memcpy(
            word,
            txt,
            len
        );

        word[len] = 0;



        int word_width =
            text_width(
                word,
                scale
            );



        /*
            Nouvelle ligne
        */
        if(
            x - start_x + word_width > width
        )
        {
            x = start_x;
            y += line_height;
        }



        text_draw(
            x,
            y,
            word,
            color,
            scale
        );


        x += word_width;



        /*
            Passage au mot suivant
        */
        while(
            *txt &&
            *txt != ' '
        )
        {
            txt++;
        }


        if(*txt == ' ')
        {
            x += (8 * scale) + 2;
            txt++;
        }
    }
}