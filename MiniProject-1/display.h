#ifndef _display_h_
#define _display_h_
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

extern void display_prompt(const char *home_dir,int prompt_flag,char last_command[]);

#endif
