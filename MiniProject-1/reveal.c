#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <errno.h>
#include <libgen.h>
#include <ctype.h>
#include "queue.h"
#include "hop.h"
#include "reveal.h"

#define RESET_COLOR "\x1b[0m"
#define GREEN_COLOR "\x1b[32m"
#define BLUE_COLOR "\x1b[34m"

int compare(const void *a, const void *b)
{
    const char *nameA = (*(struct dirent **)a)->d_name;
    const char *nameB = (*(struct dirent **)b)->d_name;

    if (strcmp(nameA, ".") == 0 || strcmp(nameA, "..") == 0)
    {
        return -1;
    }

    if (strcmp(nameB, ".") == 0 || strcmp(nameB, "..") == 0)
    {
        return 1;
    }

    return strcmp(nameA, nameB);
}

void print_permissions(struct stat *fileStat)
{
    printf((S_ISDIR(fileStat->st_mode)) ? "d" : "-");
    printf((fileStat->st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat->st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat->st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat->st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat->st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat->st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat->st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat->st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat->st_mode & S_IXOTH) ? "x" : "-");
}

void print_file_details(char *path, const char *name, int file_num)
{
    struct stat fileStat;
    char fullpath[4096];
    strcpy(fullpath, path);
    int x = strlen(path);
    fullpath[x] = '/';
    int y = strlen(name);
    for (int i = 0; i < y; i++)
    {
        fullpath[i + x + 1] = name[i];
    }
    fullpath[x + y + 1] = '\0';

    if (stat(fullpath, &fileStat) < 0)
    {
        perror("\033[1;31mStat!\n\033[0m");
        return;
    }

    printf("%d ", file_num);

    print_permissions(&fileStat);
    printf(" %ld", fileStat.st_nlink);

    struct passwd *pw = getpwuid(fileStat.st_uid);
    struct group *gr = getgrgid(fileStat.st_gid);
    printf(" %s %s", pw->pw_name, gr->gr_name);

    printf(" %5ld", fileStat.st_size);

    char time[20];
    strftime(time, 20, "%b %d %H:%M", localtime(&(fileStat.st_mtime)));
    printf(" %s", time);

    if (S_ISDIR(fileStat.st_mode))
    {
        printf(" %s%s%s\n", BLUE_COLOR, name, RESET_COLOR);
    }
    else if (fileStat.st_mode & S_IXUSR)
    {
        printf(" %s%s%s\n", GREEN_COLOR, name, RESET_COLOR);
    }
    else
    {
        printf(" %s\n",name);
    }
}

void reveal(char *path, int show_hidden, int show_details)
{
    struct stat path_stat;
    if (stat(path, &path_stat) < 0)
    {
        perror("\033[1;31mStat!\n\033[0m");
        return;
    }

    if (S_ISDIR(path_stat.st_mode))
    {
        DIR *dir;
        struct dirent *entry;
        struct dirent *namelist[1024];
        int count = 0;

        dir = opendir(path);
        if (!dir)
        {
            perror("\033[1;31mFailed to open Directory.!\n\033[0m");
            return;
        }

        while ((entry = readdir(dir)) != NULL)
        {
            if (!show_hidden && entry->d_name[0] == '.')
            {
                continue;
            }
            namelist[count] = entry;
            count++;
        }

        qsort(namelist, count, sizeof(struct dirent *), compare);

        if (show_details)
        {
            int size = 0;
            for (int i = 0; i < count; i++)
            {
                char fullpath[4096];
                strcpy(fullpath, path);
                int x = strlen(path);
                fullpath[x] = '/';
                int y = strlen(namelist[i]->d_name);
                for (int j = 0; j < y; j++)
                {
                    fullpath[j + x + 1] = (char)namelist[i]->d_name[j];
                }
                fullpath[x + y + 1] = '\0';

                struct stat entry_info;
                if (stat(fullpath, &entry_info) == 0)
                {
                    size = size + (int)(entry_info.st_blocks / 2.0);
                }
            }

            printf("total %d\n", size);
        }

        for (int j = 0; j < count; j++)
        {
            if (show_details)
            {
                print_file_details(path, namelist[j]->d_name, j + 1);
            }
            else
            {
                if (namelist[j]->d_type == DT_DIR)
                {
                    printf("%s%s%s\n", BLUE_COLOR, namelist[j]->d_name, RESET_COLOR);
                }
                else if (namelist[j]->d_type == DT_REG && access(namelist[j]->d_name, X_OK) == 0)
                {
                    printf("%s%s%s\n", GREEN_COLOR, namelist[j]->d_name, RESET_COLOR);
                }
                else
                {
                    printf("%s\n",namelist[j]->d_name);
                }
            }
        }

        closedir(dir);
    }
    else
    {
        if (show_details)
        {
            print_file_details(dirname(path), basename(path), 1);
        }
        else
        {
            printf("%s\n", basename(path));
        }
    }
}

void handle_reveal_command(char *input, const char *home_dir, char *prev_dir)
{
    int show_hidden = 0;
    int show_details = 0;
    char path[4096];
    path[0] = '\0';
    char *token = strtok(input, " ");
    while (token != NULL)
    {
        if (token[0] == '-')
        {
            if (strlen(token) == 1)
            {
                strcpy(path, "-\0");
            }

            for (int i = 1; i < strlen(token); i++)
            {
                if (token[i] == 'a')
                {
                    show_hidden = 1;
                }
                else if (token[i] == 'l')
                {
                    show_details = 1;
                }
                else
                {
                    perror("\033[1;31mInvalid Flag!\n\033[0m");
                    return ;
                }
            }
        }
        else
        {
            if (token[0] == '~')
            {
                strcpy(path, home_dir);
                strcat(path, token + 1);
            }
            else
            {
                strcpy(path, token);
            }
        }
        token = strtok(NULL, " ");
    }

    char cwd[4096];
    getcwd(cwd, sizeof(cwd));

    if (strlen(path) == 0)
    {
        strcpy(path, cwd);
    }
    else if (strcmp(path, "~") == 0)
    {
        strcpy(path, home_dir);
    }
    else if (strcmp(path, "-") == 0)
    {
        if (strcmp(prev_dir, "\0") == 0)
        {
            perror("\033[1;31mOLDPWD not set!\n\033[0m");
            return ;
        }
        printf("%s\n", prev_dir);
        strcpy(path, prev_dir);
    }

    reveal(path, show_hidden, show_details);
}
