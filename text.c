#include <stdint.h>
#include <string.h>

#include "text.h"
#include "font8x8.h"


extern uint16_t framebuffer[];

#define W 640
#define H 480



static const uint8_t *find_glyph(uint32_t c)
{
    for(unsigned int i=0;i<FONT8X8_COUNT;i++)
    {
        if(font8x8[i].c == c)
            return font8x8[i].data;
    }

    return NULL;
}



/*
    Lecture UTF-8
*/
static uint32_t utf8_next(
    const char **p
)
{
    const unsigned char *s=(const unsigned char*)*p;

    uint32_t c;


    if(s[0] < 0x80)
    {
        c=s[0];
        *p+=1;
    }
    else if((s[0]&0xe0)==0xc0)
    {
        c=((s[0]&0x1f)<<6)
        |(s[1]&0x3f);

        *p+=2;
    }
    else if((s[0]&0xf0)==0xe0)
    {
        c=((s[0]&0x0f)<<12)
        |((s[1]&0x3f)<<6)
        |(s[2]&0x3f);

        *p+=3;
    }
    else
    {
        c='?';
        *p+=1;
    }


    /*
        Majuscules accentuées
        -> majuscule simple
    */

    switch(c)
    {
        case 0x00c0:
        case 0x00c2:
            c='A';
            break;

        case 0x00c7:
            c='C';
            break;

        case 0x00c8:
        case 0x00c9:
        case 0x00ca:
            c='E';
            break;

        case 0x00ce:
            c='I';
            break;

        case 0x00d4:
            c='O';
            break;

        case 0x00d9:
        case 0x00db:
            c='U';
            break;
    }


    return c;
}



static void draw_pixel_char(
    int x,
    int y,
    uint32_t c,
    uint16_t color,
    int scale
)
{
    const uint8_t *g=find_glyph(c);


    if(!g)
    {
        g=find_glyph('?');
    }


    if(!g)
        return;



    for(int row=0;row<8;row++)
    {
        for(int col=0;col<8;col++)
        {
            if(g[row] & (0x80>>col))
            {
                for(int yy=0;yy<scale;yy++)
                {
                    for(int xx=0;xx<scale;xx++)
                    {
                        int px=x+col*scale+xx;
                        int py=y+row*scale+yy;


                        if(px>=0 && px<W &&
                           py>=0 && py<H)
                        {
                            framebuffer[py*W+px]=color;
                        }
                    }
                }
            }
        }
    }
}



int text_width(
    const char *txt,
    int scale
)
{
    int w=0;


    while(*txt)
    {
        utf8_next(&txt);

        w += (8*scale)+2;
    }


    return w;
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
        uint32_t c=utf8_next(&txt);


        draw_pixel_char(
            x,
            y,
            c,
            color,
            scale
        );


        x += (8*scale)+2;
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
    int start_x=x;


    while(*txt)
    {
        const char *old=txt;

        uint32_t c=utf8_next(&txt);



        if(c=='\n')
        {
            x=start_x;
            y += 10*scale;
            continue;
        }



        if(x+(8*scale)>width)
        {
            x=start_x;
            y += 10*scale;
        }



        draw_pixel_char(
            x,
            y,
            c,
            color,
            scale
        );


        x += (8*scale)+2;
    }
}