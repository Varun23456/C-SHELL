#ifndef _pipes_h_
#define _pipes_h_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <pwd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <grp.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include <signal.h>
#include "queue.h"
#include "hop.h"
#include "reveal.h"
#include "display.h"
#include "seek.h"
#include "proclore.h"
#include "redirection.h"
#include "activities.h"
#include "token.h"

extern void pipeline(char *temp, queue q, const char *home_dir, char *prev_dir, int flag);
#endif
