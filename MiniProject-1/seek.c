#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

int sum = 0;
int fcount = 0;
int dcount = 0;
char found_file_path[1024];
char found_dir_path[1024];


void print_colored_path(const char *path, int x)
{
    if (x == 1)
        printf("\033[1;34m./%s\033[0m\n", path); 
    else
        printf("\033[1;32m./%s\033[0m\n", path); 
}

void seek_exe(char *name, char *path, int f, int d, int e)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    char fullpath[1024];

    if ((dp = opendir(path)) == NULL)
    {
        printf("\033[1;31mCannot Open Directory!\n\033[0m");
        return;
    }

    while ((entry = readdir(dp)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        strcpy(fullpath, path);
        strcat(fullpath, "/");
        strcat(fullpath, entry->d_name);

        if (stat(fullpath, &statbuf) == -1)
        {
            continue;
        }

        int flag1 = S_ISREG(statbuf.st_mode);
        int flag2 = S_ISDIR(statbuf.st_mode);

        if ((f && flag1 && strstr(entry->d_name, name) != NULL) ||
            (d && flag2 && strstr(entry->d_name, name) != NULL))
        {
            if (flag1)
            {
                print_colored_path(fullpath + sum + 1, 0);  // sum + 1 is to move the pointer to remove the base directory and print the ralative path './file.txt'
                fcount++;
                strncpy(found_file_path, fullpath, sizeof(found_file_path));
            }
            else if (flag2)
            {
                print_colored_path(fullpath + sum + 1, 1);  // sum + 1 is to move the pointer to remove the base directory and print the ralative path './file.txt'
                dcount++;
                strncpy(found_dir_path, fullpath, sizeof(found_dir_path));
            }
        }

        if (flag2)
        {
            seek_exe(name, fullpath, f, d, e);
        }
    }
    
    closedir(dp);
    return;
}

void seekf(char *name, char *path, int f, int d, int e, char *prev_dir)
{
    sum = strlen(path);
    seek_exe(name, path, f, d, e);
    if(fcount == 0 && dcount == 0)
    {
        printf("\033[1;31mNo Match Found!\n\033[0m");
    } 

    if (e)
    {
        if (fcount == 1 && dcount == 0)
        {
            FILE *fp = fopen(found_file_path, "r");
            if (fp == NULL)
            {
                printf("\033[1;31mMissing Permissions for the Task!\n\033[0m");
                return;
            }
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), fp))
            {
                printf("%s", buffer);
            }
            fclose(fp);
        }
        else if (dcount == 1 && fcount == 0)
        {
            char temp[1024];
            getcwd(temp, sizeof(temp));
            int x = chdir(found_dir_path);
            if (x == -1)
            {
                printf("\033[1;31mFailed Changing Directory!\n\033[0m");
                fcount = 0;
                dcount = 0;
                found_file_path[0] = '\0';
                found_dir_path[0] = '\0';
                return;
            }
            else
            {
                strcpy(prev_dir, temp);
            }
        }
    }

    fcount = 0;
    dcount = 0;
    found_file_path[0] = '\0';
    found_dir_path[0] = '\0';
    return;
}
