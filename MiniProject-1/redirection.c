#include "redirection.h"

int handle_redirection(char **args, int argcount)
{
    int count = 0;
    for (int i = 0; i < argcount; i++)
    {
        if (strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0)
        {
            count++;
        }
    }

    int prev = -1;
    int var = -1;
    int flag1 = 0;
    int flag2 = 0;
    int temp1 = -1;
    int temp2 = -1;
    int temp3 = -1;
    for (int i = 0; i < argcount; i++)
    {
        if (strcmp(args[i], "<") == 0 && args[i + 1] != NULL)
        {
            if (prev == -1)
            {
                prev = i;
            }
            temp1 = open(args[i + 1], O_RDONLY);
            if (temp1 == -1)
            {
                printf("\033[1;31mNo such input file found!\n\033[0m");
                return 0;
            }
            dup2(temp1, STDIN_FILENO);
            close(temp1);
            count--;
            if (count == 0)
            {
                args[prev] = NULL;
                break;
            }
        }
        else if (strcmp(args[i], ">") == 0)
        {
            if (prev == -1)
            {
                prev = i;
            }

            flag1 = 1;
            var = i;
            if (flag2)
            {
                flag2 = 0;
            }
            if (args[i + 1] == NULL)
            {
                printf("\033[1;31mFile Name not given!\n\033[0m");
                return 0;
            }
            else
            {
                count--;
                if (count == 0)
                {
                    args[prev] = NULL;
                    break;
                }
            }
        }
        else if (strcmp(args[i], ">>") == 0 && args[i + 1] != NULL)
        {
            if (prev == -1)
            {
                prev = i;
            }

            flag2 = 1;
            var = i;
            if (flag1)
            {
                flag1 = 0;
            }

            if (args[i + 1] == NULL)
            {
                printf("\033[1;31mFile Name not given!\n\033[0m");
                return 0;
            }
            else
            {
                count--;
                if (count == 0)
                {
                    args[prev] = NULL;
                    break;
                }
            }
        }
    }

    if (flag1)
    {
        temp2 = open(args[var + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(temp2, STDOUT_FILENO);
        close(temp2);
    }

    if (flag2)
    {
        temp3 = open(args[var + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
        dup2(temp3, STDOUT_FILENO);
        close(temp3);
    }

    return 1;
}

char *f_space(char *input)
{
    while (*input == ' ')
    {
        input++;
    }
    return input;
}

char *e_space(char *input)
{
    char *end = input + strlen(input) - 1;
    while (end > input && (*end == ' ' || *end == '\t'))
    {
        end--;
    }
    end[1] = '\0';
    return input;
}

int correct_redirection(char *input)
{
    if (strstr(input, ">>") != NULL)
    {
        char *temp = strstr(input, ">>");
        char *temp2 = strstr(input, ">>");
        temp++;
        temp++;
        temp = f_space(temp);
        temp = e_space(temp);
        char path[1024];
        getcwd(path, sizeof(path));
        strcat(path, "/");
        strcat(path, temp);
        int x = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
        dup2(x, STDOUT_FILENO);
        close(x);
        *temp2 = '\0';
    }

    if (strstr(input, ">") != NULL)
    {
        char *temp = strstr(input, ">");
        char *temp2 = strstr(input, ">");
        temp++;
        temp = f_space(temp);
        temp = e_space(temp);
        char path[1024];
        getcwd(path, sizeof(path));
        strcat(path, "/");
        strcat(path, temp);
        int x = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(x, STDOUT_FILENO);
        close(x);
        *temp2 = '\0';
    }

    if (strstr(input, "<") != NULL)
    {
        char *temp = strstr(input, "<");
        char *temp2 = strstr(input, "<");
        temp++;
        temp = f_space(temp);
        temp = e_space(temp);
        char path[1024];
        getcwd(path, sizeof(path));
        strcat(path, "/");
        strcat(path, temp);
        int x = open(path, O_RDONLY);
        if (x == -1)
        {
            perror("\033[1;31mFile Not Found!\033[0m");
            return -1;
        }
        dup2(x, STDIN_FILENO);
        close(x);
        *temp2 = '\0';
    }

    return 1;
}
