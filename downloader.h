#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "store.h"


int start_download_story(Story *story);

int download_finished(void);

int download_progress(void);

int unzip_story(void);


#endif