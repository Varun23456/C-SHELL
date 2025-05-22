#include "execute.h"
int prompt_flag = 0;
char last_command[4096] = "";

int arg_split(char *input, char *args[])
{
    int count = 0;
    while (*input != '\0' && *input != '\n')
    {
        remove_space(input);

        if (*input == '\0' || *input == '\n')
        {
            break;
        }

        if (*input == '"' || *input == '\'')
        {
            input++;
            args[count] = input;

            while (*input != '"' && *input != '\0' && *input != '\n' && *input != '\'')
            {
                input++;
            }

            if (*input == '"' || *input == '\'')
            {
                char *temp = input;
                *temp = '\0';
                input = input + 1;
            }
        }
        else
        {
            args[count] = input;

            while (*input != ' ' && *input != '\t' && *input != '\0' && *input != '\n')
            {
                input++;
            }

            if (*input == ' ' || *input == '\t')
            {
                char *temp = input;
                *temp = '\0';
                input = input + 1;
            }
        }

        count++;
    }

    args[count] = NULL;
    return count;
}

void signal_handler(int sig)
{
    if (sig == SIGCHLD)
    {
        int pid;
        int status;
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
        {
            for (int i = 0; i < cnt; i++)
            {
                if (bg[i].id == pid)
                {
                    if (WIFEXITED(status))
                    {
                        printf("%s exited normally.(%d)\n", bg[i].name, pid);
                        for (int j = i; j < cnt - 1; j++)
                        {
                            bg[j] = bg[j + 1];
                        }
                        cnt--;
                    }
                    else
                    {
                        printf("%s exited abnormally.(%d)\n", bg[i].name, pid);
                    }

                    break;
                }
            }
        }
    }
}

void execute_foreground(char *input, char **args, int argcount, int f1, int f2)
{
    int pid = fork();

    if (pid < 0)
    {
        perror("\033[1;31mFork Failed!\033[0m");
    }
    else if (pid == 0)
    {
        execvp(args[0], args);
        perror("\033[1;31mExecvp Failed!\033[0m");
        exit(0);
    }
    else if (pid > 0)
    {
        dup2(f1, STDIN_FILENO);
        dup2(f2, STDOUT_FILENO);
        fg = pid;
        strcpy(fg_command, input);
        time_t t1 = time(NULL);
        int status;

        int wpid = waitpid(pid, &status, WUNTRACED);

        time_t t2 = time(NULL);
        int gap = (int)(t2 - t1);
        if (gap > 2)
        {
            prompt_flag = 1;
            strcpy(last_command, input);
        }
        fg = 0;
        fg_command[0] = '\0';
    }

    return;
}

void execute_background(char *input, char **args, int argcount, int f1, int f2)
{
    int pid = fork();

    if (pid < 0)
    {
        perror("\033[1;31mFork Failed!\033[0m");
    }
    else if (pid == 0)
    {
        setpgid(0, 0);
        execvp(args[0], args);
        perror("\033[1;31mExecvp Failed!\033[0m");
        exit(0);
    }
    else if (pid > 0)
    {
        dup2(f1, STDIN_FILENO);
        dup2(f2, STDOUT_FILENO);
        if (cnt < 100)
        {
            strcpy(bg[cnt].name, input);
            bg[cnt].id = pid;
            cnt++;
        }
        printf("[%d] %d\n", cnt, pid);
    }
}

char *remove_space(char *input)
{
    while (*input == ' ' || *input == '\t')
    {
        input++;
    }
    return input;
}

char *end_space(char *input)
{
    char *end = input + strlen(input) - 1;
    while (end > input && (*end == ' ' || *end == '\t'))
    {
        end--;
    }
    end[1] = '\0';
    return input;
}

char *select_command(char *input, queue q, const char *home_dir, char *prev_dir, int check)
{
    input = remove_space(input);
    input = end_space(input);
    input = handle_alias(input, home_dir);
    input = remove_space(input);
    input = end_space(input);
    int f1 = dup(STDIN_FILENO);
    int f2 = dup(STDOUT_FILENO);
    char file_path[4096];
    strcpy(file_path, home_dir);
    strcat(file_path, "/history.txt");
    if (correct_redirection(input) != 1)
    {
        return prev_dir;
    }
    input = remove_space(input);
    input = end_space(input);
    if (strncmp(input, "hop", 3) == 0)
    {
        if (strcmp(input, "hop") == 0)
        {
            char temp[4096];
            strcpy(temp, home_dir);
            strcpy(prev_dir, hop(temp, home_dir, prev_dir));
        }
        else
        {
            if (input[3] == '\n' || input[3] == ' ')
            {
                char *temp = input + 3;
                char *path = strtok(temp, " ");
                int flag = 0;
                while (path != NULL)
                {
                    strcpy(prev_dir, hop(path, home_dir, prev_dir));
                    path = strtok(NULL, " ");
                }
            }
            else
            {
                perror("\033[1;31mInvalid Command!\033[0m");
            }
        }

        return prev_dir;
    }
    else if (strncmp(input, "reveal", 6) == 0)
    {
        if (strlen(input) == 6)
        {
            char temp[4096];
            getcwd(temp, sizeof(temp));
            reveal(temp, 0, 0);
        }
        else
        {
            if (input[6] == '\n' || input[6] == ' ')
            {
                handle_reveal_command(input + 7, home_dir, prev_dir);
            }
            else
            {
                perror("\033[1;31mInvalid Command!\033[0m");
            }
        }
    }
    else if (strncmp(input, "log", 3) == 0)
    {
        printqueue(q);
    }
    else if (strcmp(input, "log purge") == 0)
    {
        FILE *file = fopen(file_path, "w");
        if (file == NULL)
        {
            perror("\033[1;31mError in Opening File!\033[0m");
        }

        fclose(file);

        int x = givesize(q);

        for (int i = 0; i < x; i++)
        {
            q = dequeue(q);
        }
    }
    else if (strncmp(input, "log execute ", 12) == 0)
    {
        if (strlen(input) >= 13)
        {
            char a = input[12];
            int x = a - '0';
            int y = 0;
            if (0 <= x && x <= 9)
            {
                if (input[13] == '\0')
                {
                    y = x;
                }
                else if ((0 <= input[13] - '0') && (input[13] - '0' <= 9))
                {
                    y = x * 10 + (input[13] - '0');
                }
                else
                {
                    perror("\033[1;31mInvalid Command!\033[0m");
                }
            }
            else
            {
                perror("\033[1;31mInvalid Command!\033[0m");
            }

            if (y <= 15)
            {
                char command[4096];
                int x = givesize(q);
                y = x - y;
                list temp = q->front;
                for (int i = 0; i < y; i++)
                {
                    temp = temp->next;
                }

                strcpy(command, temp->data);
                tokenise(command, q, home_dir, prev_dir);
            }
            else
            {
                perror("\033[1;31mInvalid Number!\033[0m");
                return prev_dir;
            }
        }
        else
        {
            perror("\033[1;31mInvalid Command!\033[0m");
            return prev_dir;
        }
    }
    else if (strncmp(input, "proclore", 8) == 0)
    {
        if (input[8] == '\0' || input[8] == ' ')
        {
            input = input + 8;
            char *token = strtok(input, " ");
            pid_t x = -1;
            while (token != NULL)
            {
                x = atoi(token);
                token = strtok(NULL, " ");
            }

            proclore(x);
        }
        else
        {
            perror("\033[1;31mInvalid Command!\033[0m");
        }
    }
    else if (strncmp(input, "seek", 4) == 0)
    {
        if (input[4] == '\n' || input[4] == ' ')
        {
            int f = 1;
            int d = 1;
            int e = 0;

            char name[4096];
            char direc[4096];
            direc[0] = '\0';
            int flag = 0;
            char *token;
            input = input + 4;
            token = strtok(input, " ");
            while (token != NULL)
            {
                if (token[0] == '-')
                {
                    if (token[1] == 'e')
                    {
                        e = 1;
                    }
                    else if (token[1] == 'f')
                    {
                        d = 0;
                    }
                    else if (token[1] == 'd')
                    {
                        f = 0;
                    }
                    else
                    {
                        perror("\033[1;31mInvalid Flag!\033[0m");
                    }
                }
                else
                {
                    if (flag == 0)
                    {
                        strcpy(name, token);
                        flag = 1;
                    }
                    else
                    {
                        strcpy(direc, token);
                    }
                }

                token = strtok(NULL, " ");
            }

            if (d == 0 && f == 0)
            {
                perror("\033[1;31mInvalid flags!\033[0m");
            }

            char path[4096];
            if (direc[0] == '~')
            {
                strcpy(path, home_dir);
                strcat(path, direc + 1);
            }
            else if (direc[0] == '.')
            {
                getcwd(path, sizeof(path));
                strcat(path, direc + 1);
            }
            else
            {
                if (direc[0] == '\0')
                {
                    getcwd(path, sizeof(path));
                }
                else
                {
                    strcpy(path, direc);
                }
            }

            seekf(name, path, f, d, e, prev_dir);
        }
        else
        {
            perror("\033[1;31mInvalid Command!\033[0m");
        }
    }
    else if (strcmp(input, "activities") == 0)
    {
        activities(bg, cnt);
    }
    else if (strncmp(input, "ping", 4) == 0)
    {
        input = input + 4;
        char *token;
        int pid = -1;
        int sig;
        token = strtok(input, " ");
        while (token != NULL)
        {
            if (pid == -1)
            {
                pid = atoi(token);
            }
            else
            {
                sig = atoi(token);
            }

            token = strtok(NULL, " ");
        }

        ping(pid, sig);
    }
    else if (strncmp(input, "iMan", 4) == 0)
    {
        input = input + 4;
        char *token = strtok(input, " ");
        find_ManPage(token);
    }
    else if (strncmp(input, "fg", 2) == 0)
    {
        input = input + 2;
        char *token = strtok(input, " ");
        int pid = atoi(token);
        if (pid > 0)
        {
            fg_execute(pid);
        }
        else
        {
            printf("\033[1;31mNo such process found!\033[0m\n");
        }
    }
    else if (strncmp(input, "bg", 2) == 0)
    {
        input = input + 2;
        char *token = strtok(input, " ");
        int pid = atoi(token);
        if (pid > 0)
        {
            bg_execute(pid);
        }
        else
        {
            printf("\033[1;31mNo such process found!\033[0m\n");
        }
    }
    else if (strncmp(input, "neonate -n", 10) == 0)
    {
        if(strcmp(input,"neonate -n") == 0)
        {
            printf("\033[1;31mInvalid Command!\n\033[0m");
            return prev_dir;
        }
        input = input + 10;
        char *token = strtok(input, " ");
        int time = atoi(token);
        if (time >= 0)
        {
            neonate(time);
        }
        else
        {
            printf("\033[1;31mInvalid Command!\033[0m");
        }
    }
    else if ((strcmp(input, "\n") != 0) && strlen(input) != 0)
    {
        int arg_count = 0;
        char *temp = strdup(input);
        char *args[1024];
        arg_count = arg_split(input, args);
        if (check)
        {
            execute_background(temp, args, arg_count, f1, f2);
        }
        else
        {
            execute_foreground(temp, args, arg_count, f1, f2);
        }
    }

    dup2(f1, STDIN_FILENO);
    dup2(f2, STDOUT_FILENO);
    return prev_dir;
}
