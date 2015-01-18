/**
 * @file rtmain.c
 *
 * @brief Daemon which runs the Meadow runtime.
 */
#include <stdio.h>           /* printf */
#include <unistd.h>          /* sysconf, sleep, pause */
#include <stdlib.h>          /* EXIT_SUCCESS */
#include <string.h>          /* strdup */
#include <fcntl.h>           /* O_RDWR */
#include <sys/types.h>       /* waitpid */
#include <sys/wait.h>        /* waitpid */
#include <sys/stat.h>        /* waitpid */
#include <signal.h>          /* sigaction */

#include <mq/mqueue.h>       /* mv_mqueue_t */
#include <mv/device.h>       /* mv_device_self */

#include "rtdecoder.h"       /* mvrt_decoder_new */
#include "rtsched.h"         /* mvrt_sched_new */
#include "rtevent.h"         /* mvrt_event_module_init */
#include "rtprop.h"          /* mvrt_prop_module_init */
#include "rtfunc.h"          /* mvrt_func_module_init */
#include "rtreactor.h"       /* mvrt_reactor_module_init */
#include "rtutil.h"          /* daemon_init */


extern void sched();

static void sig_child(int signo)
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

/* for now, global */
mv_mqueue_t *mq = NULL;

/* 
 * the main entry point
 */
int main(int argc, char *argv[])
{
  /* initialization for daemon */
  /*
    daemon_init(0);
  */

  if (argc < 3) {
    fprintf(stdout, "Usage: %s [name] [datafile]\n", argv[0]);
    fprintf(stdout, "  - [name]:     globally unqiue name of this device\n");
    fprintf(stdout, "  - [datafile]: file which contains device data such as "
            " properties.\n");
    exit(1);
  }

  char *self = strdup(argv[1]);
  char *datafile = strdup(argv[2]);

  /* initialize device service */
  mv_device_service_init(self, "etc/device.dat");
  
  /* load system properties, reactors, etc. */
  mvrt_obj_loadfile("etc/syslib.dat");

  /* 
   * initialize message queue handler 
   */
  mq = mv_mqueue_init(DEFAULT_PORT);
  if (mv_mqueue_run(mq) == -1) {
    fprintf(stderr, "mv_mqueue_run: failed.\n");
    exit(1);
  }
  fprintf(stdout, "Message queue thread started...\n");

  /*
   * device sign using the MQ address
   */
  mv_device_signon(self, mv_mqueue_addr(mq));
  fprintf(stdout, "Device %s signed on at %s.\n", self, mv_device_addr(self));

  /* 
   * initialize event queue 
   */
  mvrt_evqueue_t *evq = mvrt_evqueue();
  mvrt_evqueue_setcurrent(evq);

  /*
   * initialize message decoder 
   */
  mvrt_decoder_t *mf = mvrt_decoder(mq, evq);
  if (mvrt_decoder_run(mf) == -1) {
    fprintf(stderr, "mvrt_deocoder_run: failed.\n");
    exit(1);
  }
  fprintf(stdout, "Message decoder thread started...\n");

  /* 
   * initialize scheduler 
   */
  mvrt_sched_t *sched = mvrt_sched(evq);
  mvrt_sched_run(sched);

  /* Load user property, reactors, etc. 
     
     NOTE: timer initialization must happen after mv_mqueue_new, 
     mvrt_sched_run, mvrt_decoder_run are finished. This is to ensure 
     SIGRTMIN is blocked in those threads but is respected in timer.
  */
  mvrt_timer_module_init();

  mvrt_obj_loadfile(datafile);

  fprintf(stdout, "Runtime initialization finished...\n");

  /*
   * main thread perform infinite loop - Is there a better way which would
   * not wake up on SIGRTMIN?
   */
  while (1) ;

  return EXIT_SUCCESS;
}
