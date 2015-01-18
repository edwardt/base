/**
 * @file rtsched.c
 */
#include <stdio.h>       /* printf */
#include <stdlib.h>      /* malloc */
#include <pthread.h>     /* pthread_create */
#include <signal.h>      /* sigemptyset */
#include <time.h>        /* nanosleep */
#include "rtevent.h"     /* mvrt_event_t */
#include "rtreactor.h"   /* mvrt_reactor_t */
#include "rtoper.h"      /* mvrt_operator_t */
#include "rtevqueue.h"   /* mvrt_evqueue_t */
#include "rtsched.h"


/*
 * Scheduler implementation
 */
typedef struct _sched {
  pthread_t thr;
  mvrt_evqueue_t *evq;
} _sched_t;

static _sched_t *_sched_new(mvrt_evqueue_t *evq);
static int _sched_delete(_sched_t *sched);
static void *_sched_thread(void *arg);
static void _sched_exec_reactor(mvrt_reactor_t *reactor, mvrt_eventinst_t *ev);

_sched_t *_sched_new(mvrt_evqueue_t *evq)
{
  _sched_t *sched = malloc(sizeof(_sched_t));
  sched->evq = evq;

  return sched;
}

int _sched_delete(_sched_t *sched)
{
  free(sched);
  sched = NULL;

  return 0;
}

void *_sched_thread(void *arg)
{
  _sched_t *sched = (_sched_t *) arg;  /* scheduler */
  mvrt_evqueue_t *evq = sched->evq;    /* event queue */
  struct timespec ts;                  /* time for nanosleep */

  mvrt_eventinst_t *evinst;
  mvrt_event_t *ev;

  mvrt_reactor_list_t *rptr;
  mvrt_reactor_t *reactor;

  ts.tv_sec = 0;
  ts.tv_nsec = 1000;

  while (1) {
    while (mvrt_evqueue_empty(evq)) {
      nanosleep(&ts, NULL);
    }
    if ((evinst = mvrt_evqueue_get(evq)) == NULL)
      continue;

    ev = evinst->type;
    rptr = mvrt_get_reactors_for_event(ev);
    if (!rptr)
      continue;

    while (rptr) {
      reactor = rptr->reactor;
      _sched_exec_reactor(reactor, evinst);
      
      rptr = rptr->next;
    }
  }

  return NULL;
}

void _sched_exec_reactor(mvrt_reactor_t *reactor, mvrt_eventinst_t *evinst)
{
  mvrt_eval_reactor(reactor, evinst);
}


/*
 * Functions for the sched API.
 */
mvrt_sched_t *mvrt_sched(mvrt_evqueue_t *evq)
{
  _sched_t *sched = _sched_new(evq);

  return (mvrt_sched_t *) sched;
}

int mvrt_sched_delete(mvrt_sched_t *sch)
{
  _sched_t *sched = (_sched_t *) sch;

  return _sched_delete(sched);
}

int mvrt_sched_run(mvrt_sched_t *sch) 
{
  _sched_t *sched = (_sched_t *) sch;

  sigset_t sigmask;
  sigemptyset(&sigmask);
  sigaddset(&sigmask, SIGRTMIN);
  if (pthread_sigmask(SIG_BLOCK, &sigmask, NULL) != 0) {
    perror("pthread_sigmask@mvrt_sched_run");
    return -1;
  }

  if (pthread_create(&sched->thr, NULL, _sched_thread, sched) != 0) {
    perror("pthread_create@mvrt_sched_run");
    return -1;
  }

  return 0;
}
