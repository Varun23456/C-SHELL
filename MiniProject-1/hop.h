#ifndef _hop_h_
#define _hop_h_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <pwd.h>
#include <dirent.h>
#include "queue.h"
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include "queue.h"

extern char *hop(char *path, const char *home_dir, char *prev_dir);
extern char *find_relat_path(const char *home_dir);

#endif
