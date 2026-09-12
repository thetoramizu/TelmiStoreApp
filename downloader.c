#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "downloader.h"


#define TMPDIR "/mnt/SDCARD/App/TelmiStore/tmp"
#define STORIES_DIR "/mnt/SDCARD/Stories"


int start_download_story(Story *story)
{
    char cmd[1024];


    snprintf(
        cmd,
        sizeof(cmd),
        "wget -O /mnt/SDCARD/App/TelmiStore/tmp/story.zip \"%s\" "
        "> /mnt/SDCARD/App/TelmiStore/tmp/download.log 2>&1 &",
        story->download
    );


    return system(cmd);
}

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

int download_finished()
{
    return system(
        "test -s /mnt/SDCARD/App/TelmiStore/tmp/story.zip"
    )==0;
}