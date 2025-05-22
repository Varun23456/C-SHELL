#ifndef _token_h_
#define _token_h_

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
#include "execute.h"
#include "signals.h"
#include "iMan.h"
#include "fg_bg.h"
#include "neonate.h"

extern process bg[4096];
extern int cnt; 
extern int fg;
extern char fg_command[4096];
extern void tokenise(char *input, queue q, const char *home_dir, char *prev_dir);
extern char *handle_alias(char *input,const char *home_dir);
extern void replace_alias(char *input, char *name, char *output);
#endif
