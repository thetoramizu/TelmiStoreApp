#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "store.h"


int download_story(Story *story);

int start_download_story(Story *story);

int download_finished(void);


#endif