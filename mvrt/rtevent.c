/**
 * @file rtevent.c
 */
#include <stdio.h>        /* fprintf */
#include <stdlib.h>       /* free, exit */
#include <string.h>       /* strdup */
#include <assert.h>       /* assert */
#include <signal.h>       /* sigaction */
#include <time.h>         /* timer_settime */
#include <mv/device.h>    /* mv_device_self */
#include <mv/value.h>     /* mv_value_t */
#include "evqueue.h"      /* mvrt_evqueue_instance */
#include "rtevent.h"

#define MAX_RTIMER_TABLE 16
typedef struct _rtimer {
  timer_t timerid;          /* timer */
  size_t sec;               /* interval sec */
  size_t nsec;              /* interval nsec */
  mvrt_event_t rtev;        /* back pointer to _rtevent_t */
  unsigned stopped : 1;     /* timer is stopped */
  unsigned pad     : 31;    /* pad */
} _rtimer_t;
static _rtimer_t _rtimer_table[MAX_RTIMER_TABLE];

#define MAX_RTEVENT_TABLE 4096
typedef struct _rtevent {
  char *dev;
  char *name;
  union {
    mv_value_t subs;        /* LOCAL: list of subscribers */
    _rtimer_t *timer;       /* TIMER */
  } u;
  size_t noccurs;           /* number of occurrences */

  unsigned tag      : 3;    /* tag */

  unsigned free     : 1;    /* free or not */
  unsigned free_idx : 12;   /* index to free list */
  unsigned free_nxt : 12;   /* index of next free entry */
  unsigned pad      : 4;
} _rtevent_t;
static _rtevent_t _rtevent_table[MAX_RTEVENT_TABLE];
static _rtevent_t *_free_rtevent;

static int _rtevent_table_init();
static _rtevent_t *_rtevent_get_free();
static int _rtevent_delete(_rtevent_t *rtev);
static _rtevent_t *_rtevent_lookup(const char *dev, const char *name);

static _rtimer_t *_rtimer_create(const char *name, size_t sec, size_t nsec);
static void _rtimer_handler(int sig, siginfo_t *sinfo, void *uc);

int _rtevent_table_init()
{
  int i;
  _rtevent_t *ev;
  for (i = 0; i < MAX_RTEVENT_TABLE; i++) {
    ev = _rtevent_table + i;
    ev->dev = NULL;
    ev->name = NULL;

    ev->free = 1;
    ev->free_idx = i;
    ev->free_nxt = i + 1;
  }
  /* the last element */
  ev->free_idx = i;
  ev->free_nxt = 0;

  _free_rtevent = _rtevent_table + 1;
  _rtevent_table[0].free = 0;
}

_rtevent_t *_rtevent_get_free()
{
  if (_free_rtevent == _rtevent_table)
    return NULL;

  _rtevent_t *rtev = _free_rtevent;
  _free_rtevent = _rtevent_table + rtev->free_nxt;
  rtev->free = 0;

  return rtev;
}

int _rtevent_delete(_rtevent_t *rtev)
{
  rtev->free = 1;
  rtev->free_nxt = _free_rtevent->free_idx;
  _free_rtevent = rtev;

  return 0;
}

_rtevent_t *_rtevent_lookup(const char *dev, const char *name)
{
  int i;
  _rtevent_t *rtev;
  for (i = 1; i < MAX_RTEVENT_TABLE; i++) {
    rtev = _rtevent_table + i;
    if (!rtev->free && !strcmp(rtev->dev, dev) && !strcmp(rtev->name, name))
      return rtev;
  }

  return NULL;
}

static size_t _rtimerid = 0;
_rtimer_t *_rtimer_create(const char *name, size_t sec, size_t nsec)
{
  struct sigevent sev;
  struct itimerspec its;

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

  return rtimer;
}

extern mvrt_evqueue_t *mvrt_evqueue_getcurrent();
static void _rtimer_handler(int sig, siginfo_t *sinfo, void *uc)
{
  int i;
  timer_t *tid;
  _rtimer_t *rtimer;

  struct timespec ts; /* 1ms */
  ts.tv_sec = 0;
  ts.tv_nsec = 1000; 
  
  mvrt_evqueue_t *evq = mvrt_evqueue_getcurrent();
  tid = sinfo->si_value.sival_ptr;
  for (i = 0; i < _rtimerid; i++) {
    rtimer = _rtimer_table + i;
    if (*tid == rtimer->timerid) {
      mvrt_event_t rtev = rtimer->rtev;
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


/*
 * Functions for events.
 */
int mvrt_event_module_init()
{
  _rtevent_table_init();
}

mvrt_event_t mvrt_event_new(const char *dev, const char *name, int tag)
{
  _rtevent_t *rtev;
  if ((rtev = _rtevent_get_free()) == NULL) {
    fprintf(stderr, "mvrt_event_new: MAX table reached.\n");
    return (mvrt_event_t) 0;
  }
  if (rtev->dev) free(rtev->dev);
  if (rtev->name) free(rtev->name);
  rtev->dev = strdup(dev);
  rtev->name = strdup(name);
  rtev->tag = tag;
  rtev->noccurs = 0;

  switch (tag) {
  case MVRT_EVENT_GLOBAL:
  case MVRT_EVENT_SYSTEM:
  case MVRT_EVENT_LOCAL:
  case MVRT_EVENT_TIMER:
    return (mvrt_event_t) rtev;
  default:
    assert(0 && "mvrt_event_new: Invalid tag");
  }

  return (mvrt_event_t) 0;
}

int mvrt_event_delete(mvrt_event_t ev)
{
  return 0;
}

mvrt_event_t mvrt_event_lookup(const char *dev, const char *name)
{
  int i;
  _rtevent_t *rtev;
  if ((rtev = _rtevent_lookup(dev, name)) == NULL)
    return (mvrt_event_t) 0;

  return (mvrt_event_t) rtev;
}

const char *mvrt_event_dev(mvrt_event_t event)
{
  _rtevent_t *rtev = (_rtevent_t *) event;
  return rtev->dev;
}

const char *mvrt_event_name(mvrt_event_t event)
{
  _rtevent_t *rtev = (_rtevent_t *) event;
  return rtev->name;
}

mvrt_eventag_t mvrt_event_tag(mvrt_event_t event)
{
  _rtevent_t *rtev = (_rtevent_t *) event;
  return rtev->tag;
}

/*
 * Functions for timers.
 */
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

mvrt_event_t mvrt_timer_new(const char *name, size_t sec, size_t nsec)
{
  fprintf(stdout, "Create timer: %s (%d sec %d nsec)\n", name, sec, nsec);
  const char *dev = mv_device_self();
  _rtevent_t *rtev = (_rtevent_t *) mvrt_event_new(dev, name, MVRT_EVENT_TIMER);
  rtev->u.timer = _rtimer_create(name, sec, nsec);
  rtev->u.timer->sec = sec;
  rtev->u.timer->nsec = nsec;
  rtev->u.timer->rtev = (mvrt_event_t) rtev;
  rtev->u.timer->stopped = 0;

  return (mvrt_event_t) rtev;
}

int mvrt_timer_start(mvrt_event_t event)
{
  _rtevent_t *rtev = (_rtevent_t *) event;
  assert(rtev->tag == MVRT_EVENT_TIMER);
  rtev->u.timer->stopped = 0;
}

int mvrt_timer_stop(mvrt_event_t event)
{
  _rtevent_t *rtev = (_rtevent_t *) event;
  assert(rtev->tag == MVRT_EVENT_TIMER);
  rtev->u.timer->stopped = 1;
}


/*
 * Functions for event instances.
 */
mvrt_eventinst_t *mvrt_eventinst_new(mvrt_event_t ev, mv_value_t data)
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
  return 0;}

