#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
#include "execute.h"
#include "activities.h"
#include "signals.h"
#include "iMan.h"
#include "fg_bg.h"
#include "neonate.h"

int f1;
int f2;
int exit_flag = 0;

void ctrl_C(int sig)
{
    if (fg == 0)
    {
        // printf("\n");
        return;
    }

    strcpy(bg[cnt].name, fg_command);
    bg[cnt].id = fg;
    cnt++;

    int x;
    if (kill(fg, 0) == 0)
    {
        x = kill(fg, sig);
        if (x == 0)
        {
            printf("\n%s [%d] is Terminated.\n", fg_command, fg);
            return;
        }
    }

    if (x == -1)
    {
        if (errno == ESRCH)
        {
            printf("No such process found\n");
        }
        else
        {
            perror("Error sending signal");
        }
    }

    return;
}

void ctrlZ(int sig)
{

    if (fg == 0)
    {
        printf("\n");
        return;
    }

    strcpy(bg[cnt].name, fg_command);
    bg[cnt].id = fg;
    cnt++;

    int x;
    if (kill(fg, 0) == 0)
    {
        x = kill(fg, sig);
        if (x == 0)
        {
            printf("\n\033[1;31m[%d]+        Stopped           %s\033[0m\n", fg, fg_command);
            return;
        }
    }

    if (x == -1)
    {
        if (errno == ESRCH)
        {
            printf("No such process found\n");
        }
        else
        {
            perror("Error sending signal");
        }
    }
}

int main()
{
    f1 = dup(STDIN_FILENO);
    f2 = dup(STDOUT_FILENO);
    char home_dir[4096];
    char file_path[4096];
    getcwd(home_dir, sizeof(home_dir));
    char prev_dir[4096];

    strcpy(prev_dir, "\0");

    queue q = createqueue();

    strcpy(file_path, home_dir);
    strcat(file_path, "/history.txt");

    FILE *file = fopen(file_path, "r");
    if (file != NULL)
    {
        char var[4096];
        while (fgets(var, sizeof(var), file))
        {
            int x = strlen(var);
            var[x - 1] = '\0';
            q = enqueue(q, var);
        }
    }

    while (1)
    {
        signal(SIGCHLD, signal_handler);
        signal(SIGINT, ctrl_C);
        signal(SIGTSTP, ctrlZ);
        display_prompt(home_dir, prompt_flag, last_command);
        prompt_flag = 0;
        last_command[0] = '\0';
        char input[4096];
        int s = 0;
        while (1)
        {
            char ch;
            char temp = scanf("%c", &ch);
            if (temp == EOF) // to handle ctrl+d
            {
                printf("\n");
                exit_flag = 1;
                for (int i = 0; i < cnt; i++)
                {
                    kill(bg[i].id, SIGKILL);
                    printf("'%s'[%d] killed.\n", bg[i].name, bg[i].id);
                }
                break;
            }

            if (ch == '\n')
            {
                break;
            }

            input[s] = ch;
            s++;
        }
        input[s] = '\0';

        if (exit_flag == 1)
        {
            break;
        }

        if (strstr(input, "log") == NULL)
        {
            int x = givesize(q);
            list temp = q->rear;

            if (!(temp != NULL && strcmp(temp->data, input) == 0))
            {
                if (x == 15)
                {
                    q = dequeue(q);
                }

                q = enqueue(q, input);
                file_write(q, file_path);
            }
        }

        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        tokenise(input, q, home_dir, prev_dir);

        fflush(stdout);
    }

    return 0;
}
