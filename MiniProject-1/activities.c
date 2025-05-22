#include "activities.h"

int compare_processes(const void *a, const void *b)
{
    const process *p1 = (const process *)a;
    const process *p2 = (const process *)b;
    return (p1->id - p2->id);
}

char *getProcessState(int pid)
{
    if (kill(pid, 0) == -1)
    {
        if (errno == ESRCH)
        {
            return "Terminated";
        }
    }

    char path[100];
    strcpy(path, "/proc/");
    char str[100];
    strcpy(str, convert(pid));
    strcat(path, str);
    strcat(path, "/stat");

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        return "Unknown";
    }

    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) == NULL)
    {
        printf("\033[1;31mFailed to read stat file!\n\033[0m");
        fclose(file);
        return "Unknown";
    }

    fclose(file);
    int ppid, pgrp, session, tty_nr, tpgid;
    long unsigned int vsize;
    char comm[256], state;

    sscanf(buffer, "%d %s %c %d %d %d %d %d %lu", &pid, comm, &state, &ppid, &pgrp, &session, &tty_nr, &tpgid, &vsize);

    if (state == 'R' || state == 'S' || state == 'Z')
    {
        return "Running";
    }
    else if (state == 'D')
    {
        return "Waiting";
    }
    else if (state == 'T')
    {
        return "Stopped";
    }
    else
    {
        return "Unknown";
    }
}

void activities(process bg[], int cnt)
{
    qsort(bg, cnt, sizeof(process), compare_processes);
    if(cnt == 0)
    {
        printf("\033[1;31mNo Background Processes Running!\n\033[0m");
        return;
    }
    for (int i = 0; i < cnt; i++)
    {
        printf("%d : %s - %s\n", bg[i].id, bg[i].name, getProcessState(bg[i].id));
    }
    return;
}
