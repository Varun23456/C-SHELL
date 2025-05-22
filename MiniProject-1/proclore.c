#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

char *convert(int pid)
{
    int s = 0;
    int x = pid;
    if (x < 0)
    {
        x = -x;
    }

    while (x >= 1)
    {
        s++;
        x = x / 10;
    }

    char *result;
    if (pid >= 0)
    {
        result = (char *)malloc((s + 1) * sizeof(char));
    }
    else
    {
        result = (char *)malloc((s + 2) * sizeof(char));
    }

    int y = pid;
    if (y < 0)
    {
        y = -y;
    }

    if (pid >= 0)
    {
        result[s] = '\0';
        s--;
    }
    else
    {
        result[s + 1] = '\0';
    }

    while (y >= 1)
    {
        int z = y % 10;
        z += 48;
        result[s] = (char)z;
        s--;
        y = y / 10;
    }

    if (pid < 0)
    {
        result[0] = '-';
    }

    return result;
}

void proclore(pid_t pid)
{
    if(pid == -1)
    {
        pid = getpid();
    }
    char path[256];
    char buffer[1024];
    strcpy(path, "/proc/");
    char str[20];
    strcpy(str,convert(pid));
    strcat(path, str);
    strcat(path, "/stat");

    FILE *file = fopen(path, "r");

    if (!file)
    {
        printf("\033[1;31mFailed to open state file!\n\033[0m");
        return;
    }

    if (fgets(buffer, sizeof(buffer), file) == NULL)
    {
        printf("\033[1;31mFailed to read stat file!\n\033[0m");
        fclose(file);
        return;
    }

    fclose(file);

    int ppid, pgrp, session, tty_nr, tpgid;
    long unsigned int vsize;
    char comm[256], state;

    sscanf(buffer, "%d %s %c %d %d %d %d %d %lu", &pid, comm, &state, &ppid, &pgrp, &session, &tty_nr, &tpgid, &vsize);
    printf("pid : %d\n", pid);
    
    if (pgrp == tpgid)
    {
        if (state == 'S' || state == 'R')
        {
            printf("process Status : %c+\n", state);
        }
        else
        {
            printf("process Status : %c\n", state);
        }
    }
    else
    {
        printf("process Status : %c\n", state);
    }

    printf("Process Group : %d\n", pgrp);
    printf("Virtual memory : %lu bytes\n", vsize);
    if (state == 'Z')
    {
        return;
    }

    strcpy(path, "/proc/");
    char str2[20];
    sprintf(str2, "%d", pid);
    strcat(path, str2);
    strcat(path, "/exe");

    char exe_path[1024];
    ssize_t len = readlink(path, exe_path, sizeof(exe_path) - 1);

    if (len != -1)
    {
        exe_path[len] = '\0';
    }
    else
    {
        printf("\033[1;31mFailed to read exe link!\n\033[0m");
        return;
    }

    printf("executable path : %s\n", exe_path);
    return;
}

