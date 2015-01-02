/**
 * @file rtsched.c
 *
 * @author cjeong
 */
#include <stdio.h>       /* printf */
#include <stdlib.h>      /* malloc */
#include <pthread.h>     /* pthread_create */
#include <time.h>        /* nanosleep */
#include "rtevent.h"     /* mvrt_event_t */
#include "evqueue.h"     /* mvrt_evqueue_t */
#include "reactor.h"     /* mvrt_reactor_t */
#include "operator.h"    /* mvrt_operator_t */
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
static void _sched_exec_reactor(mvrt_reactor_t *reactor, mvrt_value_t event);

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

  mvrt_value_t nullval = mvrt_value_null();
  mvrt_value_t eventval;
  mvrt_event_t ev;

  mvrt_reactor_list_t *rptr;
  mvrt_reactor_t *reactor;

  ts.tv_sec = 0;
  ts.tv_nsec = 1000;

  while (1) {
    while (mvrt_evqueue_empty(evq)) {
      nanosleep(&ts, NULL);
    }
    if ((eventval = mvrt_evqueue_get(evq)) == nullval) 
      continue;

    ev = mvrt_value_event_type(eventval);
    rptr = mvrt_get_reactors_for_event(ev);
    if (!rptr)
      continue;

    while (rptr) {
      reactor = rptr->reactor;
      _sched_exec_reactor(reactor, eventval);
      
      rptr = rptr->next;
    }
  }

  return NULL;
}

void _sched_exec_reactor(mvrt_reactor_t *reactor, mvrt_value_t ev)
{
  mvrt_eval_reactor(reactor, ev);
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
  
  if (pthread_create(&sched->thr, NULL, _sched_thread, sched) != 0) {
    perror("pthread_create@mvrt_sched_run");
    return -1;
  }

  return 0;
}
