#ifndef _proclore_h_
#define _proclore_h_

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

extern void proclore(pid_t pid);
extern char *convert(int pid);
#endif
