/**
 * @file rtutil.c
 */
#include <stdio.h>            /* printf */
#include <unistd.h>           /* sysconf, sleep */
#include <stdlib.h>           /* EXIT_SUCCESS */
#include <fcntl.h>            /* O_RDWR */
#include <sys/types.h>        /* waitpid */
#include <sys/wait.h>         /* waitpid */
#include <sys/stat.h>
#include <signal.h>           /* sigaction */
#include "rtutil.h"

static void _sig_child(int signo);
static int _daemon_init(int flags);

static void _sig_child(int signo)
{
  pid_t pid;
  int status;
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    printf("child %d terminated\n", pid);
 
  if (pid == -1) {
    /* child processes not fully cleaned up: possibility of zombies */
    perror("waitpid@sig_child");
  }
  return;
}

static int _daemon_init(int flags)
{
  /* see TLPI for details */
  int maxfd, fd;
  if (!(flags & BD_NO_CLOSE_FILES)) {
    if ((maxfd = sysconf(_SC_OPEN_MAX)) == -1) {
      /* if limit is indeterminate, guess it */
      maxfd = BD_MAX_CLOSE; 
    }
    for (fd = 0; fd < maxfd; fd++)
      close(fd);
  }

  /* redirect standard fd's to /dev/null */
  if (!(flags & BD_NO_REDIRECT_STD_FDS)) {
    close(STDIN_FILENO);
    fd = open("/dev/null", O_RDWR);
    if (fd != STDIN_FILENO)
      return -1;
    if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
      return -1;
    if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
      return -1;
  }

  return 0;
}

int daemon_init(int flags) 
{
  _daemon_init(flags);
  register_signal_handler(SIGCHLD, _sig_child);
}

void register_signal_handler(int signo, void (*fptr)(int)) 
{
  struct sigaction act, oldact;
  act.sa_handler = fptr;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  if (signo == SIGCHLD) {
    act.sa_flags |= SA_RESTART;
  }
  if (sigaction(signo, &act, &oldact) == -1)
    return;
}

