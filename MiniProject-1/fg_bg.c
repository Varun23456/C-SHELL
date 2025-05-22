#include "fg_bg.h"

void bg_execute(int pid)
{
    if (kill(pid, 0) == 0)
    {
        if (strcmp(getProcessState(pid), "Stopped") == 0)
        {
            kill(pid, SIGCONT);
        }
        else
        {
            perror("\033[1;31mGiven process is already running in Background!\033[0m\n");
        }
    }
    else
    {
        perror("\033[1;31mNo such process found!\033[0m\n");
    }
}

void fg_execute(int pid)
{
    char *temp;
    for (int i = 0; i < cnt; i++)
    {
        if (pid == bg[i].id)
        {
            fg = bg[i].id;
            strcpy(fg_command,bg[i].name);
            temp = strdup(bg[i].name);
            for (int j = i; j < cnt - 1; j++)
            {
                bg[j] = bg[j + 1];
            }
            cnt--;
        }
    }

    // int shell_terminal = STDIN_FILENO;
    // int shell_pgid = getpgrp();
    // int temp1 = tcsetpgrp(shell_terminal, pid);
    // if(temp1 == -1)
    // {
    //     printf("\033[1;31mFailed to change process group!\033[0m\n");
    // }
    // signal(SIGTTIN, SIG_IGN);
    // signal(SIGTTOU, SIG_IGN);

    if (kill(pid, 0) == 0)
    {
        int x = kill(pid, SIGCONT);
        if (x == -1)
        {
            printf("033[1;31mError in continuing process!\033[0m\n");
        }
    }
    else
    {
        printf("\033[1;31mNo such process found!\033[0m\n");
    }

    time_t t1 = time(NULL);
    int status;
    waitpid(pid, &status, WUNTRACED);

    // temp1 = tcsetpgrp(shell_terminal, shell_pgid);

    // if(temp1 == -1)
    // {
    //     perror("033[1;31mFailed to restore shell process group!\033[0m\n");
    // }

    // signal(SIGTTIN, SIG_IGN);
    // signal(SIGTTOU, SIG_IGN);
    time_t t2 = time(NULL);
    int gap = (int)(t2 - t1);
    if (gap > 2)
    {
        prompt_flag = 1;
        strcpy(last_command, temp);
    }
    return;
}
