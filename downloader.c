#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "downloader.h"


#define TMPDIR "/mnt/SDCARD/App/TelmiStore/tmp"
#define STORIES_DIR "/mnt/SDCARD/Stories"


int download_story(Story *story)
{
    char cmd[1024];


    /*
        Téléchargement ZIP
    */

    snprintf(
        cmd,
        sizeof(cmd),
        "mkdir -p %s && "
        "wget -O %s/story.zip \"%s\"",
        TMPDIR,
        TMPDIR,
        story->download
    );


    int ret = system(cmd);


    if(ret != 0)
        return 0;



    /*
        Vérification ZIP
    */

    snprintf(
        cmd,
        sizeof(cmd),
        "test -s %s/story.zip",
        TMPDIR
    );


    ret = system(cmd);


    if(ret != 0)
        return 0;



    /*
        Décompression
    */

    snprintf(
        cmd,
        sizeof(cmd),
        "busybox unzip -o %s/story.zip -d %s",
        TMPDIR,
        STORIES_DIR
    );


    ret = system(cmd);


    if(ret != 0)
        return 0;



    return 1;
}