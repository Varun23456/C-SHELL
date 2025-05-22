#include "pipes.h"

void pipeline(char *temp, queue q, const char *home_dir, char *prev_dir, int flag)
{
    printf("%s\n",temp);
    int f1 = dup(STDIN_FILENO);
    int f2 = dup(STDOUT_FILENO);
    char input[1024];
    strcpy(input, temp);
    if (flag)
    {
        int x = strlen(temp);
        input[x] = '&';
        input[x + 1] = '\0';
    }

    char * var = strdup(input);
    if (strlen(input) == 0)
    {
        perror("\033[1;31mInvalid piping!\033[0m");
        return;
    }

    while (*var == ' ' || *var == '\t')
    {
        var++;
    }

    if(*var == '|')
    {
        perror("\033[1;31mInvalid piping!\033[0m");
        return;
    }

    char *commands[1024];
    int num_pipes = 0;
    char *saveptr;
    char *token = strtok_r(var, "|", &saveptr);
    while (token != NULL)
    {
        commands[num_pipes++] = strdup(token);
        token = strtok_r(NULL, "|", &saveptr);
    }
    commands[num_pipes] = NULL;

    if(num_pipes == 1)
    {
        perror("\033[1;31mInvalid piping!\033[0m");
        return;
    }

    for (int i = 0; i < num_pipes; i++)
    {
        while (*commands[i] == ' ' || *commands[i] == '\t')
        {
            commands[i]++;
        }

        char *end = commands[i] + strlen(commands[i]) - 1;
        while (end > commands[i] && (*end == ' ' || *end == '\t'))
        {
            end--;
        }
        end[1] = '\0';
        if (strlen(commands[i]) == 0)
        {
            perror("\033[1;31mInvalid piping!\033[0m");
            return;
        }
    }

    int pipe_fds[2 * (num_pipes - 1)];

    for (int i = 0; i < num_pipes - 1; i++)
    {
        if (pipe(pipe_fds + i * 2) == -1)
        {
            perror("pipe");
            exit(0);
        }
    }

    for (int i = 0; i < num_pipes; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            if (i > 0)
            {
                dup2(pipe_fds[(i - 1) * 2], STDIN_FILENO);
            }

            if (i < num_pipes - 1)
            {
                dup2(pipe_fds[i * 2 + 1], STDOUT_FILENO);
            }

            for (int j = 0; j < 2 * (num_pipes - 1); j++)
            {
                close(pipe_fds[j]);
            }

            tokenise(commands[i], q, home_dir, prev_dir);
            exit(0);
        }
        else
        {
            usleep(500 * 100);
        }

    }

    for (int i = 0; i < 2 * (num_pipes - 1); i++)
    {
        close(pipe_fds[i]);
    }

    for (int i = 0; i < num_pipes; i++)
    {
        wait(NULL);
    }

    dup2(f1, STDIN_FILENO);
    dup2(f2, STDOUT_FILENO);

    return;
}
