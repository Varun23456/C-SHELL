#include "neonate.h"

int max(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int recentpid()
{
    DIR *dir;
    int id = -1;

    if ((dir = opendir("/proc")) == NULL)
    {
        perror("opendir");
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (atoi(entry->d_name) > 0)
        {
            int temp = atoi(entry->d_name);
            id = max(temp, id);
        }
    }

    closedir(dir);
    return id;
}

void set_terminal(int flag)
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (flag)
    {
        tty.c_lflag &= ~(ICANON | ECHO);
    }
    else
    {
        tty.c_lflag |= (ICANON | ECHO);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

int check_key()
{
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    int x = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return x;
}

void neonate(int time)
{
    set_terminal(1);

    while (true)
    {
        int pid = recentpid();
        if (pid > 0)
        {
            printf("%d\n", pid);
        }

        int term_flag = 0;
        for (int i = 0; i < 100 * time; i++)
        {
            usleep(10000);
            if (check_key())
            {
                char ch = getchar();
                if (ch == 'x' || ch == 'X')
                {
                    term_flag = 1;
                    break;
                }
            }
        }

        if (check_key()) // because, if time = 0 it wont enter 'for' loop.
        {
            char ch = getchar();
            if (ch == 'x' || ch == 'X')
            {
                term_flag = 1;
                break;
            }
        }

        if (term_flag)
        {
            break;
        }
    }

    set_terminal(0);
    return;
}
