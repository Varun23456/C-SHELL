#ifndef _seek_h_
#define _seek_h_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include "queue.h"
#include "hop.h"
#include "reveal.h"
#include "display.h"

extern void seek_exe(char *name,char *path, int f, int d, int e);
extern void seekf(char *name, char *path, int f, int d, int e,char *prev_dir);

#endif
