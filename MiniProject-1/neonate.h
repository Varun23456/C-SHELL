#ifndef _neonate_h_
#define _neonate_h_
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
#include <termios.h>
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

extern int recentpid();
extern void set_terminal(int flag);
extern int check_key();
extern void neonate(int time);

#endif
