#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "downloader.h"


#define TMPDIR "/mnt/SDCARD/App/TelmiStore/tmp"
#define STORIES_DIR "/mnt/SDCARD/Stories"
#define RUNFILE TMPDIR "/download.running"

#define ZIPFILE TMPDIR "/story.zip"
#define TMPZIP  TMPDIR "/story.tmp"
#define LOGFILE TMPDIR "/download.log"



int start_download_story(Story *story)
{
    char cmd[1024];


snprintf(
    cmd,
    sizeof(cmd),
    "rm -f %s %s %s %s && "
    "touch %s && "
    "wget -O %s \"%s\" > %s 2>&1 && "
    "mv %s %s && "
    "rm -f %s &",
    ZIPFILE,
    TMPZIP,
    LOGFILE,
    RUNFILE,

    RUNFILE,

    TMPZIP,
    story->download,
    LOGFILE,

    TMPZIP,
    ZIPFILE,

    RUNFILE
);

printf("CMD=%s\n", cmd);
    return system(cmd);
}



int download_finished()
{
    return
        system(
            "test -s " ZIPFILE " && "
            "! test -e " RUNFILE
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
        char *p=strchr(line,'%');


        if(p)
        {
            int value=0;
            int multiplier=1;


            p--;


            while(p>=line && *p>='0' && *p<='9')
            {
                value += (*p-'0') * multiplier;
                multiplier *= 10;
                p--;
            }


            if(value>=0 && value<=100)
                percent=value;
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