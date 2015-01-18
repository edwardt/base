/**
 * @file rtevqueue.c
 */
#include <stdio.h>       /* sprintf */
#include <stdlib.h>      /* malloc */
#include <string.h>      /* memcpy */
#include <time.h>        /* nanosleep */
#include <errno.h>       /* errno */
#include <assert.h>      /* assert */
#include "rtevqueue.h"


#define MAX_EVENT_QUEUE 4096
typedef struct _evqueue_struct {
  pthread_mutex_t lock;
  int size;
  int head;
  int tail;
  mvrt_eventinst_t **evs;
} _evqueue_t;

static _evqueue_t *_evq_inst = NULL;

static _evqueue_t *_evqueue_new(int size);
static int _evqueue_delete(_evqueue_t *evq);
static int _evqueue_enqueue(_evqueue_t *evq, mvrt_eventinst_t *evinst);
static mvrt_eventinst_t *_evqueue_dequeue(_evqueue_t *evq);

_evqueue_t *_evqueue_new(int size)
{
  if (_evq_inst) {
    fprintf(stderr, "An event queue already exists. Currently, only one "
            "event queue is supported.\n");
    return NULL;
  }

  if (size > MAX_EVENT_QUEUE) {
    fprintf(stderr, "Max event queue size is %d.\n", MAX_EVENT_QUEUE);
    size = MAX_EVENT_QUEUE;
  }
  _evqueue_t *evq = malloc(sizeof(_evqueue_t));
  pthread_mutex_init(&evq->lock, NULL);
  evq->size = size;
  evq->head = 0;
  evq->tail = 0;
  evq->evs = malloc(sizeof(mvrt_eventinst_t *) * size);

  return evq;
}

int _evqueue_delete(_evqueue_t *evq)
{
  free(evq->evs);
  free(evq);
  
  return 0;
}

int _evqueue_full(_evqueue_t *evq)
{
  return (evq->tail + 1) % evq->size == evq->head;
}

int _evqueue_empty(_evqueue_t *evq)
{
  return evq->tail == evq->head;
}

int _evqueue_enqueue(_evqueue_t *evq, mvrt_eventinst_t *evinst)
{
  if (pthread_mutex_lock(&evq->lock) != 0) {
    perror("pthread_mutex_lock@_evqueue_enqueue");
    return -1;
  }

  if (_evqueue_full(evq)) {
    if (pthread_mutex_unlock(&evq->lock) != 0) {
      perror("pthread_mutex_unlock@_evqueue_enqueue");
    }
    return -1;
  }

  evq->evs[evq->tail] = evinst;
  evq->tail = (evq->tail + 1) % evq->size;

  if (pthread_mutex_unlock(&evq->lock) != 0)  {
    perror("pthread_mutex_unlock@_evqueue_enqueue");
    return -1;
  }

  return 0;
}

mvrt_eventinst_t *_evqueue_dequeue(_evqueue_t *evq)
{
  pthread_mutex_lock(&evq->lock);
  if (_evqueue_empty(evq)) {
    pthread_mutex_unlock(&evq->lock);
    return NULL;
  }

  mvrt_eventinst_t *evinst = evq->evs[evq->head];
  evq->head = (evq->head + 1) % evq->size;

  pthread_mutex_unlock(&evq->lock);
  return evinst;
}

/*
 * Functions for the evqueue interface.
 */
mvrt_evqueue_t *mvrt_evqueue()
{
  _evqueue_t *evq = _evqueue_new(MAX_EVENT_QUEUE);

  return (mvrt_evqueue_t *) evq;
}

static mvrt_evqueue_t *_current_evq = NULL;
mvrt_evqueue_t *mvrt_evqueue_setcurrent(mvrt_evqueue_t *evq)
{
  _current_evq = evq;
}

mvrt_evqueue_t *mvrt_evqueue_getcurrent()
{
  return _current_evq;
}

mvrt_eventinst_t *mvrt_evqueue_get(mvrt_evqueue_t *q)
{
  if (!q)
    return NULL;

  _evqueue_t *evq = (_evqueue_t *) q;

  return _evqueue_dequeue(evq);
}

int mvrt_evqueue_put(mvrt_evqueue_t *q, mvrt_eventinst_t *evinst)
{
  if (!evinst)
    return -1;

  _evqueue_t *evq = (_evqueue_t *) q;

  return _evqueue_enqueue(evq, evinst);
}

int mvrt_evqueue_full(mvrt_evqueue_t *q)
{
  _evqueue_t *evq = (_evqueue_t *) q;

  return _evqueue_full(evq);
}

int mvrt_evqueue_empty(mvrt_evqueue_t *q)
{
  _evqueue_t *evq = (_evqueue_t *) q;

  return _evqueue_empty(evq);
}
