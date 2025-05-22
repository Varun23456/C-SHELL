#include "signals.h"
void ping(int pid, int sig)
{
    sig = sig % 32;
    int x;
    x = kill(pid,0);
    if(x == -1)
    {
        printf("No process.\n");
    }
    
    if (kill(pid, 0) == 0)
    { 
        x = kill(pid, sig);
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
    else
    {
        printf("Sent signal %d to process with pid %d\n", sig, pid);
    }

    return;
}

