/**
 * @file main.c
 *
 * @brief Daemon which runs the Meadow runtime.
 */
#include <stdio.h>           /* printf */
#include <unistd.h>          /* sysconf, sleep */
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

#include "message.h"
#include "mdecoder.h"
#include "rtsched.h"
#include "rtevent.h"
#include "rtprop.h"
#include "operator.h"
#include "rtfunc.h"
#include "reactor.h"

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


char *dest;

/* registers system events */
static mvrt_event_t ncall_ev;
static void register_system_events()
{
  const char *self = mv_device_self();
  mvrt_event_new(self, ":prop_add", MVRT_EVENT_SYSTEM);
  mvrt_event_new(self, ":prop_get", MVRT_EVENT_SYSTEM);
  mvrt_event_new(self,":prop_set", MVRT_EVENT_SYSTEM);

  mvrt_event_new(self, ":func_call", MVRT_EVENT_SYSTEM);
  ncall_ev = mvrt_event_new(self, ":native_call", MVRT_EVENT_SYSTEM);
}

static mvrt_event_t timer0;
static void register_user_events()
{
  timer0 = mvrt_timer_new("timer0", 10);
}

static void register_system_funcs()
{
}

static mvrt_func_t turnon;
static void register_user_funcs()
{
  turnon = mvrt_func_new(dest, "gpioPut", MVRT_FUNC_GLOBAL);
}


static void register_system_props()
{
}

static void register_user_props()
{
  const char *self = mv_device_self();

  mvrt_prop_t prev = mvrt_prop_new(self, "pval", MVRT_PROP_LOCAL);
  mvrt_prop_t curr = mvrt_prop_new(self, "cval", MVRT_PROP_LOCAL);
  mvrt_value_t prev_init = mvrt_value_int(0);
  mvrt_value_t curr_init = mvrt_value_int(0);
  mvrt_prop_setvalue(prev, prev_init);
  mvrt_prop_setvalue(curr, curr_init);
}

mv_mqueue_t *mq;

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

  if (argc < 3) {
    fprintf(stdout, "Usage: %s [device name] [dest device]\n", argv[0]);
    exit(1);
  }
  char *self = strdup(argv[1]);
  dest = strdup(argv[2]);

  /* initialize device service */
  mv_device_service_init(self, "device.dat");

  /* initialize other remote services */
  mv_prop_service_init((char *) 0);
  mv_event_service_init((char *) 0);
  mv_func_service_init((char *) 0);
  
  /* initialize local event module */
  mvrt_event_module_init();
  register_system_events();
  register_user_events();

  /* initialize local function module */
  mvrt_func_module_init((char *) "native.dat");
  register_system_funcs();
  register_user_funcs();

  /* initialize local prop module */
  mvrt_prop_module_init((char *) 0);
  register_system_props();
  register_user_props();

  /* initialize reactor module */
  mvrt_reactor_module_init((char *) "reactor.dat");

  /* initialize message queue handler */
  mq = mv_mqueue_init(DEFAULT_PORT);
  if (mv_mqueue_run(mq) == -1) {
    fprintf(stderr, "mv_mqueue_run: failed.\n");
    exit(1);
  }
  fprintf(stdout, "Message queue thread started...\n");

  /* device sign on */
  mv_device_signon(self, mv_mqueue_addr(mq));
  fprintf(stdout, "Device %s signed on at %s.\n", self, mv_device_addr(self));

  /* initialize event queue */
  mvrt_evqueue_t *evq = mvrt_evqueue();
  mvrt_evqueue_setcurrent(evq);

  /* initialize message decoder */
  mvrt_mdecoder_t *mf = mvrt_mdecoder(mq, evq);
  if (mvrt_mdecoder_run(mf) == -1) {
    fprintf(stderr, "mvrt_mdeocoder_run: failed.\n");
    exit(1);
  }
  fprintf(stdout, "Message decoder thread started...\n");

  /* initialize scheduler */
  mvrt_sched_t *sched = mvrt_sched(evq);
  mvrt_sched_run(sched);

  /* add reactors to events */
  mvrt_reactor_t *r0 = mvrt_reactor_lookup_by_name("native_call_reactor");
  mvrt_reactor_t *r1 = mvrt_reactor_lookup_by_name("r1");
  mvrt_add_reactor_to_event(ncall_ev, r0);
  mvrt_add_reactor_to_event(timer0, r1);

  /* temporarily, generate timer event manually */
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_sec = 2;
  ts.tv_nsec = 300000; // 0.3s
  while (1) {
    nanosleep(&ts, NULL);

    /*
      mv_value_t null_v = mv_value_null();
      mvrt_value_t eval = mvrt_value_event(timer0, null_v);
      
      while (mvrt_evqueue_full(evq)) {
      nanosleep(&ts, NULL);
      }
      mvrt_evqueue_put(evq, eval);
    */

  }

  return EXIT_SUCCESS;
}
