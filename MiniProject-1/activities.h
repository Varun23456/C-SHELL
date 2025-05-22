#ifndef _activity_h_
#define _activity_h_
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
#include "pipes.h"
#include "token.h"
#include "execute.h"

extern int compare_processes(const void *a, const void *b);
extern char *getProcessState(int pid);
extern void activities(process bg[],int cnt);

#endif
