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

void display_prompt(const char *home_dir, int prompt_flag, char last_command[])
{
    char hostname[4096];
    char *username;
    char *temp = (char *)malloc(sizeof(char) * 4096);

    username = getpwuid(getuid())->pw_name;
    gethostname(hostname, sizeof(hostname));

    temp = find_relat_path(home_dir);
    if (!prompt_flag)
    {
        printf("\033[32m<%s@%s\033[0m:\033[34m%s>\033[0m ", username, hostname, temp);
    }
    else
    {
        printf("\033[32m<%s@%s\033[0m:\033[34m%s %s>\033[0m ", username, hostname, temp,last_command);
    }
    free(temp);
}
