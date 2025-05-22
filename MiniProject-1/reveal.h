#ifndef _reveal_h_
#define _reveal_h_

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

extern int compare(const void *a, const void *b);
extern void print_permissions(struct stat *fileStat);
extern void print_file_details(char *path, const char *name, int file_num);
extern void reveal(char *path, int show_hidden, int show_details);
extern void handle_reveal_command(char *input, const char *home_dir, char *prev_dir);

#endif

