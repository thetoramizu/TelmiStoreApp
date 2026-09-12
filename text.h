#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>


/*
    Affichage texte simple
*/
void text_draw(
    int x,
    int y,
    const char *txt,
    uint16_t color,
    int scale
);



/*
    Affichage texte avec retour automatique
*/
void text_draw_wrap(
    int x,
    int y,
    int width,
    const char *txt,
    uint16_t color,
    int scale
);



/*
    Calcule la largeur d'un texte
*/
int text_width(
    const char *txt,
    int scale
);


#endif