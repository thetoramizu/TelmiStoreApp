#ifndef STORE_H
#define STORE_H

#define MAX_STORIES 64


typedef struct
{
    char title[128];
    char description[512];
    char download[256];

    int installed;

} Story;



extern Story stories[MAX_STORIES];
extern int story_count;



int store_load(void);


#endif