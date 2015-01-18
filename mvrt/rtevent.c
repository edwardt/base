/**
 * @file rtevent.c
 */
#include <stdio.h>        /* fprintf */
#include <stdlib.h>       /* free, exit, atoi */
#include <string.h>       /* strdup */
#include <assert.h>       /* assert */
#include <signal.h>       /* sigaction */
#include <time.h>         /* timer_settime */
#include <mv/device.h>    /* mv_device_self */
#include <mv/value.h>     /* mv_value_t */
#include "rtevqueue.h"    /* mvrt_evqueue_instance */
#include "rtevent.h"
#include "rtobj.h"


#define MAX_RTIMER_TABLE 16
typedef struct _rtimer {
  timer_t timerid;        /* timer */
  size_t sec;             /* interval sec */
  size_t nsec;            /* interval nsec */
  mvrt_event_t *rtev;     /* back pointer to mvrt_event_t */
  unsigned stopped : 1;   /* timer is stopped */
  unsigned used    : 1;   /* used */
  unsigned pad     : 30;  /* pad */
} _rtimer_t;
static _rtimer_t _rtimer_table[MAX_RTIMER_TABLE];

static _rtimer_t *_rtimer_new(size_t sec, size_t nsec);
static int _rtimer_delete(_rtimer_t *timer);
static void _rtimer_handler(int sig, siginfo_t *sinfo, void *uc);

static int __rtevent_tokenize(char *line, char **, char **, char **, char **);
static _rtimer_t *_rtevent_parse(char *line, char **type, char **name);


static size_t _rtimerid = 0;
_rtimer_t *_rtimer_new(size_t sec, size_t nsec)
{
  struct sigevent sev;
  struct itimerspec its;

  /* TODO: reused empty spot in the table */
  if (_rtimerid == MAX_RTIMER_TABLE) {
    fprintf(stderr, "Cannot create more timers.\n");
    return NULL;
  }
  _rtimer_t *rtimer = _rtimer_table + _rtimerid++;

  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = SIGRTMIN;
  sev.sigev_value.sival_ptr = &rtimer->timerid;

  timer_create(CLOCK_REALTIME, &sev, &rtimer->timerid);

  its.it_interval.tv_sec = sec;
  its.it_interval.tv_nsec = nsec;
  its.it_value.tv_sec = sec;
  its.it_value.tv_nsec = nsec;

  timer_settime(rtimer->timerid, 0, &its, NULL);

  rtimer->sec = sec;
  rtimer->nsec = nsec;
  rtimer->stopped = 0;
  rtimer->used = 1;

  return rtimer;
}

int _rtimer_delete(_rtimer_t *rtimer)
{
  /* TODO: do real deletion */
  rtimer->stopped = 1;
  rtimer->used = 0;
}

extern mvrt_evqueue_t *mvrt_evqueue_getcurrent();
static void _rtimer_handler(int sig, siginfo_t *sinfo, void *uc)
{
  int i;
  timer_t *tid;
  _rtimer_t *rtimer;
  
  printf("rtimer_handler\n");
  struct timespec ts; /* 1ms */
  ts.tv_sec = 0;
  ts.tv_nsec = 1000; 
  
  mvrt_evqueue_t *evq = mvrt_evqueue_getcurrent();
  tid = sinfo->si_value.sival_ptr;
  for (i = 0; i < _rtimerid; i++) {
    rtimer = _rtimer_table + i;
    if (*tid == rtimer->timerid) {
      mvrt_event_t *rtev = rtimer->rtev;
      if (rtimer->stopped)
        continue;

      mv_value_t evdata = mv_value_null();
      mvrt_eventinst_t *ev = mvrt_eventinst_new(rtev, evdata);

      while (mvrt_evqueue_full(evq))
        nanosleep(&ts, NULL);

      mvrt_evqueue_put(evq, ev);
    }
  }
}

int _rtevent_tokenize(char *line, char **type, char **name, char **s, char **ns)
{
  char *token;

  /* get type: "event" or "timer" */
  if ((token = strtok(line, " \t")) == NULL)
    return -1;
  *type = token;

  /* get "name" */
  if ((token = strtok(NULL, " \t")) == NULL)
    return -1;
  *name = token;

  /* get "sec nsec" for timers */
  if (!strcmp(*type, "timer")) {
    if ((token = strtok(NULL, " \t")) == NULL)
      return -1;
    *s = token;

    if ((token = strtok(NULL, " \t")) == NULL)
      return -1;
    *ns = token;
  }
  else {
    if (strcmp(*type, "event")) 
      return -1;
  }

  return 0;
}

/* In case the line specifies a timer, it returns a valid rtimer struct
   on success -- otherwise, returns NULL. In case the line specifies an
   event, the type points to string "event" -- otherwise, sets type to NULL. */
_rtimer_t *_rtevent_parse(char *line, char **type, char **name)
{
  /* event ev0
     event ev1
     timer timer0 1 1000
     timer timer1 0 1000000
  */

  char *arg0;
  char *arg1;
  if (_rtevent_tokenize(line, type, name, &arg0, &arg1) == -1) {
    fprintf(stderr, "Line not recognized: %s\n", line);
    *type = NULL;
    return NULL;
  }

  _rtimer_t *rtimer = NULL;
  if (!strcmp(*type, "timer"))
    rtimer = _rtimer_new(atoi(arg0), atoi(arg1));

  return rtimer;
}


/*
 * Functions for rtevent API.
 */
mvrt_event_t *mvrt_event_new(const char *name, const char *dev)
{
  mvrt_obj_t *obj = mvrt_obj_lookup(name, dev);
  if (obj) {
    if (!dev) {
      fprintf(stderr, "An event already exists with name: %s.\n", name);
      return NULL;
    }
    else
      return (mvrt_event_t *) obj;
  }
  
  obj = mvrt_obj_new(name, dev);
  obj->tag = MVRT_OBJ_EVENT;

  /* Regular events do not need data. The existence of "event object" itself 
     suffices. */
  obj->data = NULL;

  return (mvrt_event_t *) obj;
}

mvrt_event_t *mvrt_timer_new(const char *name, size_t sec, size_t nsec)
{
  mvrt_obj_t *obj = mvrt_obj_lookup(name, NULL);
  if (obj) {
    fprintf(stderr, "A timer already exists with name: %s.\n", name);
    return NULL;
  }
  
  obj = mvrt_obj_new(name, NULL);
  obj->tag = MVRT_OBJ_EVENT;

  _rtimer_t *rtimer = NULL;
  if ((rtimer = _rtimer_new(sec, nsec)) == NULL) {
    mvrt_obj_delete(obj);
    return NULL;
  }

  obj->data = (void *) rtimer;
  rtimer->rtev = (mvrt_event_t *) obj;

  return (mvrt_event_t *) obj;
}

mvrt_event_t *mvrt_event_load_str(char *line)
{
  _rtimer_t *rtimer;
  char *type = NULL;
  char *name = NULL;
  if ((rtimer = _rtevent_parse(line, &type, &name)) == NULL && type == NULL)
    return NULL;

  mvrt_obj_t *obj = mvrt_obj_lookup(name, NULL);
  if (obj) {
    fprintf(stderr, "A runtime object already exists with name: %s.\n", name);
    return NULL;
  }
  obj = mvrt_obj_new(name, NULL);
  obj->tag = MVRT_OBJ_EVENT;

  if (rtimer) {
    obj->data = (void *) rtimer;
    rtimer->rtev = (mvrt_event_t *) obj;
  }

  return (mvrt_event_t *) obj;
}

int mvrt_event_delete(mvrt_event_t *ev)
{
  mvrt_obj_t *obj = (mvrt_obj_t *) ev;

  _rtimer_t *timer = (_rtimer_t *) obj->data;
  if (timer && _rtimer_delete(timer) == -1)
    return -1;

  return mvrt_obj_delete(obj);
}

char *mvrt_event_save_str(mvrt_event_t *ev)
{
  static char str[4096];

  mvrt_obj_t *obj = (mvrt_obj_t *) ev;
  if (!obj)
    return NULL;

  _rtimer_t *timer = (_rtimer_t *) obj->data;
  if (timer) {
    int sec = timer->sec;
    int nsec = timer->nsec;
    if (snprintf(str, 4096, "timer %s %d %d", obj->name, sec, nsec) > 4095) {
      fprintf(stderr, "Buffer overflow.\n");
      return NULL;
    }
  }
  else {
    if (snprintf(str, 4096, "event %s", obj->name) > 4095) {
      fprintf(stderr, "Buffer overflow.\n");
      return NULL;
    }
  }

  return &str[0];
}

int mvrt_event_delete_by_name(const char *name)
{
  mvrt_obj_t *obj = mvrt_obj_lookup(name, NULL);

  return mvrt_event_delete((mvrt_event_t *) obj);
}

mvrt_event_t *mvrt_event_lookup(const char *name, const char *dev)
{
  mvrt_obj_t *obj = mvrt_obj_lookup(name, dev);
  if (!obj)
    return NULL;

  assert(obj->tag == MVRT_OBJ_EVENT);

  return (mvrt_event_t *) obj;
}


int mvrt_timer_module_init()
{
  struct sigaction sa;

  sigset_t sigmask;
  sigemptyset(&sigmask);
  sigaddset(&sigmask, SIGRTMIN);
  if (pthread_sigmask(SIG_UNBLOCK, &sigmask, NULL) != 0) {
    perror("sigprocmask@mvrt_timer_module_init");
    return -1;
  }

  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = _rtimer_handler;
  sigemptyset(&sa.sa_mask);
  if (sigaction(SIGRTMIN, &sa, NULL) != 0) {
    fprintf(stderr, "Failed to set up signal handler for timers.\n");
    return -1;
  }

  fprintf(stdout, "Timer module initialized...\n");

  return 0;
}

int mvrt_timer_start(mvrt_event_t *ev)
{
  mvrt_obj_t *obj = (mvrt_obj_t *) ev;
  assert(obj->tag == MVRT_OBJ_EVENT && obj->data);

  _rtimer_t *timer = obj->data;
  timer->stopped = 0;

  return 0;
}

int mvrt_timer_stop(mvrt_event_t* ev)
{
  mvrt_obj_t *obj = (mvrt_obj_t *) ev;
  assert(obj->tag == MVRT_OBJ_EVENT && obj->data);

  _rtimer_t *timer = obj->data;
  timer->stopped = 1;

  return 0;
}


/*
 * Functions for event instances.
 */
mvrt_eventinst_t *mvrt_eventinst_new(mvrt_event_t *ev, mv_value_t data)
{
  /* TODO: rather than directly doing malloc, we can maintain an instance
     pool to reuse preallocated instance object, keep track of memory
     for event instances, etc. */

  mvrt_eventinst_t *evinst = malloc(sizeof(mvrt_eventinst_t));
  evinst->type = ev;
  evinst->data = data;

  return evinst;
}

int mvrt_eventinst_delete(mvrt_eventinst_t *evinst)
{
  free(evinst);
  return 0;

}
