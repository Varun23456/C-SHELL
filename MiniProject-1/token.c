#include "token.h"
process bg[4096];
int cnt = 0;
int fg = 0;
char fg_command[4096];

void replace_alias(char *input, char *name, char *output)
{
    FILE *file = fopen(name, "r");
    if (file == NULL)
    {
        perror("Failed to open alias file");
        return;
    }

    char line[4096];
    char * x = (char *)malloc(4096 * sizeof(char));
    char * y = (char *)malloc(4096 * sizeof(char));
    char *saveptr2;
    char *token = strtok_r(input, " ", &saveptr2);
    output[0] = '\0';
    int var = 1;

    while (token != NULL)
    {
        int flag = 1;
        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (strcmp(line, "\n") != 0 || line == NULL)
            {
                char *saveptr;
                char *temp = strtok_r(line, "=", &saveptr);
                strcpy(x, temp);
                temp = strtok_r(NULL, "=", &saveptr);
                strcpy(y, temp);
                x = remove_space(x);
                x = end_space(x);
                y = remove_space(y);
                y = end_space(y);
                if (y[strlen(y) - 1] == '\n')
                {
                    y[strlen(y) - 1] = '\0';
                }
                
                if (strcmp(token, x) == 0)
                {
                    flag = 0;
                    if (var == 1)
                    {
                        strcpy(output, y);
                        strcat(output, " ");
                        break;
                    }
                    strcat(output, y);
                    strcat(output, " ");
                    break;
                }
            }
        }

        if (flag != 0)
        {
            strcat(output, token);
            strcat(output, " ");
        }

        var++;
        token = strtok_r(NULL, " ", &saveptr2);
        rewind(file);
    }

    fclose(file);
    return;
}

char *handle_alias(char *input,const char *home_dir)
{
    char *temp = strdup(input);
    char *output = (char *)malloc(1024 * sizeof(char));
    char name[4096];
    strcpy(name,home_dir);
    strcat(name,"/.myshrc");
    replace_alias(temp, name, output);
    return output;
}

void tokenise(char *input, queue q, const char *home_dir, char *prev_dir)
{
    if (strstr(input, "||") != NULL)
    {
        perror("\033[1;31mInvalid piping!\033[0m");
        return;
    }
    char *temp1;
    char *temp2;
    char *part1 = strtok_r(input, ";", &temp1);
    while (part1 != NULL)
    {

        int count = 0;
        int x = strlen(part1);
        for (int i = 0; i < x; i++)
        {
            if (part1[i] == '&')
            {
                count++;
            }
        }
        char *part2 = strtok_r(part1, "&", &temp2);

        while (part2 != NULL)
        {
            while (*part2 == ' ' || *part2 == '\t')
            {
                part2++;
            }

            char *end = part2 + strlen(part2) - 1;
            while (end > part2 && (*end == ' ' || *end == '\t'))
            {
                end--;
            }
            end[1] = '\0';

            if (*part2 != '\0')
            {
                if (count > 0)
                {
                    if (strstr(part2, "|") != NULL)
                    {
                        pipeline(part2, q, home_dir, prev_dir, 1);
                        part2 = strtok_r(NULL, "&", &temp2);
                        continue;
                    }
                    strcpy(prev_dir, select_command(part2, q, home_dir, prev_dir, 1));
                    count--;
                }
                else
                {
                    if (strstr(part2, "|") != NULL)
                    {
                        pipeline(part2, q, home_dir, prev_dir, 0);
                        part2 = strtok_r(NULL, "&", &temp2);
                        continue;
                    }
                    strcpy(prev_dir, select_command(part2, q, home_dir, prev_dir, 0));
                }
            }

            part2 = strtok_r(NULL, "&", &temp2);
        }

        part1 = strtok_r(NULL, ";", &temp1);
    }

    return;
}
