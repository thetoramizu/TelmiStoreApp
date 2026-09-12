#include "text.h"
#include "font8x8.h"

#include <string.h>
#include <stdint.h>


extern void rect(
    int x,
    int y,
    int w,
    int h,
    uint16_t color
);



/*
    Retourne le prochain caractère UTF-8
    et avance le pointeur.
*/
static uint32_t utf8_next(const char **s)
{
    const unsigned char *p =
        (const unsigned char *)*s;


    uint32_t c;


    if(p[0] < 0x80)
    {
        c = p[0];
        *s += 1;
    }

    else if((p[0] & 0xe0) == 0xc0)
    {
        c =
            ((p[0] & 0x1f) << 6) |
            (p[1] & 0x3f);

        *s += 2;
    }

    else if((p[0] & 0xf0) == 0xe0)
    {
        c =
            ((p[0] & 0x0f) << 12) |
            ((p[1] & 0x3f) << 6) |
            (p[2] & 0x3f);

        *s += 3;
    }

    else
    {
        c='?';
        *s += 1;
    }


    return c;
}



/*
    Conversion caractères français
    vers glyphes internes.
*/
static char convert_char(uint32_t c)
{
    switch(c)
    {
        case 0x00e9: // é
        case 0x00e8: // è
        case 0x00ea: // ê
        case 0x00eb: // ë
        case 0x00c9: // É
            return 'E';


        case 0x00e0: // à
        case 0x00e2: // â
        case 0x00e4: // ä
        case 0x00c0:
            return 'A';


        case 0x00e7: // ç
        case 0x00c7:
            return 'C';


        case 0x00f9: // ù
        case 0x00fb: // û
        case 0x00fc: // ü
        case 0x00d9:
            return 'U';


        case 0x00f4: // ô
        case 0x00f6: // ö
        case 0x00d4:
            return 'O';


        case 0x00ee: // î
        case 0x00ef: // ï
        case 0x00ce:
            return 'I';


        default:

            if(c < 128)
            {
                char ch=(char)c;

                if(ch>='a' && ch<='z')
                    ch-=32;

                return ch;
            }

            return '?';
    }
}



static const uint8_t *get_glyph(char c)
{
    for(unsigned int i=0;i<FONT8X8_COUNT;i++)
    {
        if(font8x8[i].c==c)
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
    const uint8_t *g =
        get_glyph(c);


    if(!g)
    {
        rect(
            x,
            y,
            8*scale,
            8*scale,
            0xf800
        );

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



int text_width(
    const char *txt,
    int scale
)
{
    int width=0;


    while(*txt)
    {
        const char *old=txt;

        uint32_t c=utf8_next(&txt);


        (void)c;


        if(txt==old)
            break;


        width += (8*scale)+2;
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
        uint32_t c =
            utf8_next(&txt);


        char out =
            convert_char(c);


        draw_char(
            x,
            y,
            out,
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
    int start_x = x;

    int line_height =
        (8 * scale) + 6;


    char word[128];


    while(*txt)
    {
        int bytes = 0;


        /*
            Trouve la longueur du mot en OCTETS
        */
        while(
            txt[bytes] &&
            txt[bytes] != ' '
        )
        {
            bytes++;
        }


        if(bytes >= sizeof(word))
            bytes = sizeof(word)-1;


        memcpy(
            word,
            txt,
            bytes
        );

        word[bytes]=0;



        int word_width =
            text_width(
                word,
                scale
            );



        /*
            Retour ligne
        */
        if(
            x - start_x + word_width > width
        )
        {
            x=start_x;
            y+=line_height;
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
            Saute le mot dans le texte original
        */
        txt += bytes;


        /*
            Mange l'espace
        */
        if(*txt==' ')
        {
            txt++;

            x += (8*scale)+2;
        }
    }
}