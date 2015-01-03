/**
 * @file main.c
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
#include <support/daemon.h>  /* daemon_init */

#include "mdecoder.h"
#include "rtsched.h"
#include "rtevent.h"
#include "rtprop.h"
#include "rtfunc.h"
#include "reactor.h"
#include "sysinit.h"

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
     register_signal_handler(SIGCHLD, sig_child);
  */

  if (argc < 2) {
    fprintf(stdout, "Usage: %s [device name]\n", argv[0]);
    exit(1);
  }
  char *self = strdup(argv[1]);

  /* initialize device service */
  mv_device_service_init(self, "device.dat");
  
  /* initialize local event module */
  mvrt_event_module_init();
  mvrt_system_event_init();

  /* initialize local function module */
  mvrt_func_module_init((char *) "native.dat");
  mvrt_system_func_init();

  /* initialize local prop module */
  mvrt_prop_module_init((char *) 0);
  mvrt_system_prop_init();

  /* initialize reactor module */
  mvrt_reactor_module_init((char *) "reactor.dat");
  mvrt_system_reactor_init();

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
  mvrt_mdecoder_t *mf = mvrt_mdecoder(mq, evq);
  if (mvrt_mdecoder_run(mf) == -1) {
    fprintf(stderr, "mvrt_mdeocoder_run: failed.\n");
    exit(1);
  }
  fprintf(stdout, "Message decoder thread started...\n");

  /* 
   * initialize scheduler 
   */
  mvrt_sched_t *sched = mvrt_sched(evq);
  mvrt_sched_run(sched);

  /* 
   * Hardcode user-defined events, props, etc.
   *
   * NOTE: timer initialization must happen after mv_mqueue_new, 
   * mvrt_sched_run, mvrt_decoder_run is finished. This is to ensure 
   * SIGRTMIN is blocked in those threads but is respected in timer.
   */
  /* user events */
  mvrt_timer_module_init();
  mvrt_event_t timer0 = mvrt_timer_new("timer0", 1);

  /* user props */
  mvrt_prop_t prev = mvrt_prop_new(self, "pval", MVRT_PROP_LOCAL);
  mvrt_prop_t curr = mvrt_prop_new(self, "cval", MVRT_PROP_LOCAL);
  mv_value_t prev_init = mv_value_int(0);
  mv_value_t curr_init = mv_value_int(0);
  mvrt_prop_setvalue(prev, prev_init);
  mvrt_prop_setvalue(curr, curr_init);

  /* add reactors to events */
  mvrt_reactor_t *r1 = mvrt_reactor_lookup("r1");
  mvrt_add_reactor_to_event(timer0, r1);

  /*
   * main thread perform infinite loop - Is there a better way?
   */
  while (1) ;

  return EXIT_SUCCESS;
}
