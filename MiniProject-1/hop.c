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
#include "hop.h"

char *find_relat_path(const char *home_dir)
{
    char cwd[4096];
    char *relative_path = (char *)malloc(sizeof(char) * 4096);

    getcwd(cwd, sizeof(cwd));
    int x = strlen(home_dir);
    int i = 0;
    int flag = 1;
    while (i < x)
    {
        if (cwd[i] != home_dir[i])
        {
            flag = 0;
            break;
        }
        i++;
    }

    if (flag)
    {
        if (strcmp(cwd, home_dir) == 0)
        {
            strcpy(relative_path, "~");
        }
        else
        {
            relative_path[0] = '~';
            int y = strlen(cwd);
            int i = x;
            int z = 1;
            while (i < y)
            {
                relative_path[z] = cwd[i];
                z++;
                i++;
            }
            relative_path[z] = '\0';
        }
    }
    else
    {
        strcpy(relative_path, cwd);
    }

    return relative_path;
}

char *hop(char *path, const char *home_dir, char *prev_dir)
{
    char temp[4096];
    char var[4096];

    if (strncmp(path, "~", 1) == 0)
    {
        strcpy(var, home_dir);
        if (strlen(path) > 1)
        {
            strcat(var, path + 1);
        }
    }
    else if (strcmp(path, "-") == 0)
    {
        if(strcmp(prev_dir,"\0") == 0)
        {
            printf("\033[1;31mOLDPWD not set!\n\033[0m");
            return prev_dir;
        }
        strcpy(var, prev_dir);
    }
    else
    {
        strcpy(var, path);
    }

    getcwd(temp, sizeof(temp));

    int x = chdir(var);
    if (x == -1)
    {
        printf("\033[1;31mHop Failed!\n\033[0m");
    }
    else
    {
        strcpy(prev_dir, temp);
        getcwd(temp,sizeof(temp));
        printf("%s\n",temp);
    }

    return prev_dir;
}
