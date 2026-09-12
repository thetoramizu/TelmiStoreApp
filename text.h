#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>


/*
    Dessine une chaîne simple
*/
void text_draw(
    int x,
    int y,
    const char *txt,
    uint16_t color,
    int scale
);


/*
    Dessine une chaîne avec retour automatique
    à la ligne.
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
    Retourne la largeur approximative d'un texte
*/
int text_width(
    const char *txt,
    int scale
);


#endif