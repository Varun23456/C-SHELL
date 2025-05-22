#ifndef _redirection_h_
#define _redirection_h_
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
#include <fcntl.h>
#include "queue.h"

extern int handle_redirection(char **args, int argcount);
extern int correct_redirection(char *input);
#endif
