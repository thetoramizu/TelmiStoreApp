#include "store.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Story stories[MAX_STORIES];
int story_count = 0;



static int extract_value(
    const char *src,
    const char *key,
    char *out,
    int max
)
{
    char pattern[64];

    snprintf(
        pattern,
        sizeof(pattern),
        "\"%s\"",
        key
    );


    char *p=strstr(src,pattern);

    if(!p)
        return 0;


    p=strchr(p,':');

    if(!p)
        return 0;


    p++;


    while(*p==' ' || *p=='\"')
        p++;


    int i=0;


    while(
        *p &&
        *p!='\"' &&
        i<max-1
    )
    {
        out[i++]=*p++;
    }


    out[i]=0;


    return 1;
}



int store_load(void)
{
    system(
        "wget -q -O store.json https://storetelmiseb.netlify.app/store.json"
    );


    FILE *f=fopen(
        "store.json",
        "rb"
    );


    if(!f)
    {
        printf("store.json absent\n");
        return 0;
    }



    fseek(
        f,
        0,
        SEEK_END
    );


    long size=ftell(f);


    rewind(f);



    char *json=
        malloc(size+1);


    if(!json)
    {
        fclose(f);
        return 0;
    }



    fread(
        json,
        1,
        size,
        f
    );


    json[size]=0;


    fclose(f);



    char *p=json;


    while(
        (p=strstr(p,"\"title\"")) &&
        story_count<MAX_STORIES
    )
    {
        Story *s=&stories[story_count];


        memset(
            s,
            0,
            sizeof(Story)
        );


        extract_value(
            p,
            "title",
            s->title,
            sizeof(s->title)
        );


        extract_value(
            p,
            "description",
            s->description,
            sizeof(s->description)
        );


        extract_value(
            p,
            "download",
            s->download,
            sizeof(s->download)
        );


        s->installed=0;


        story_count++;


        p+=7;
    }



    free(json);



    printf(
        "Stories chargees: %d\n",
        story_count
    );


    return story_count;
}