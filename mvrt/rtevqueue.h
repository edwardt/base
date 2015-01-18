/**
 * @file rtevqueue.h
 *
 * @brief Interface to runtime event queues.
 */
#ifndef MVRT_EVQUEUE_H
#define MVRT_EVQUEUE_H

#include "rtevent.h"    /* mvrt_eventinst_t */


typedef void mvrt_evqueue_t;

/* Create a new event queue which will send and receive events from
   message decoder and encoder. */
extern mvrt_evqueue_t *mvrt_evqueue();

/* Sets/returns the "current instance" of evenet queue.
   TODO: is it advantageous to have multiple event queues? */
extern void *mvrt_evqueue_setcurrent(mvrt_evqueue_t *evq);
extern mvrt_evqueue_t *mvrt_evqueue_getcurrent();

/* Delete the event queue. It will stop the event queue and discard all 
   buffered events. All subsequent calls to mvrt_evqueue_get or 
   mvrt_evqueue_put calls over the deleted queue will fail. Returns 0 on 
   sucess and -1 on failure. */
extern int mvrt_evqueue_delete(mvrt_evqueue_t *evq);

/* Start/stop running the event queue. Returns 0 on success and -1 on 
   failure. */
extern int mvrt_evqueue_run(mvrt_evqueue_t *evq);
extern int mvrt_evqueue_stop(mvrt_evqueue_t *evq);

/* Blocking get. Returns -1 on failure. */
extern mvrt_eventinst_t *mvrt_evqueue_get(mvrt_evqueue_t *evq);

/* Blocking put. Returns -1 on failure. */
extern int mvrt_evqueue_put(mvrt_evqueue_t *evq, mvrt_eventinst_t *ev);

/* Returns 1 iff the event queue is full. */
extern int mvrt_evqueue_full(mvrt_evqueue_t *evq);

/* Returns 1 iff the event queue is empty. */
extern int mvrt_evqueue_empty(mvrt_evqueue_t *evq);

#endif /* MVRT_EVQUEUE_H */
