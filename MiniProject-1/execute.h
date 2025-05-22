#ifndef _execute_h_
#define _execute_h_
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
#include "activities.h"
#include "signals.h"
#include "iMan.h"
#include "fg_bg.h"
#include "neonate.h"

extern int prompt_flag;
extern char last_command[4096];
extern char *select_command(char *input, queue q, const char *home_dir, char *prev_dir, int check);
extern void signal_handler(int sig);
extern void execute_foreground(char *input, char **args, int argcount, int f1, int f2);
extern char* remove_space(char *input);
extern char * end_space(char *input);
extern void execute_background(char *input, char **args, int argcount, int f1, int f2);
extern int arg_split(char *input, char *args[]);

#endif
