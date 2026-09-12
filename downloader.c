#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "downloader.h"


#define TMPDIR "/mnt/SDCARD/App/TelmiStore/tmp"
#define STORIES_DIR "/mnt/SDCARD/Stories"

#define ZIPFILE TMPDIR "/story.zip"
#define TMPZIP  TMPDIR "/story.tmp"
#define LOGFILE TMPDIR "/download.log"



int start_download_story(Story *story)
{
    char cmd[1024];


    snprintf(
        cmd,
        sizeof(cmd),
        "rm -f %s %s %s && "
        "wget "
        "-O %s \"%s\" "
        "> %s 2>&1 && "
        "mv %s %s &",
        ZIPFILE,
        TMPZIP,
        LOGFILE,
        TMPZIP,
        story->download,
        LOGFILE,
        TMPZIP,
        ZIPFILE
    );


    return system(cmd);
}



int download_finished()
{
    return system(
        "test -s " ZIPFILE
    )==0;
}



int download_progress()
{
    FILE *f;


    f=fopen(
        LOGFILE,
        "r"
    );


    if(!f)
        return 0;


    char line[256];

    int percent=0;


    while(fgets(line,sizeof(line),f))
    {
        int p;


        if(sscanf(
            line,
            "%*[^0-9]%d%%",
            &p
        )==1)
        {
            percent=p;
        }
    }


    fclose(f);


    return percent;
}



int unzip_story()
{
    char cmd[512];


    snprintf(
        cmd,
        sizeof(cmd),
        "busybox unzip -o %s -d %s",
        ZIPFILE,
        STORIES_DIR
    );


    return system(cmd)==0;
}