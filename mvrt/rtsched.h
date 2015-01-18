/**
 * @file sched.h
 *
 * @brief Interface to scheduler.
 */
#ifndef MVRT_SCHED_H
#define MVRT_SCHED_H

#include "rtevqueue.h"   /* mvrt_evqueue_t */


typedef void mvrt_sched_t;

/* Create a new scheduler which looks at the given event queue for any
   new event occurrence. */
extern mvrt_sched_t *mvrt_sched(mvrt_evqueue_t *evq);

/* Delete the scheduler. */
extern int mvrt_sched_delete(mvrt_sched_t *sched);

/* Start/stop running the scheduler. Returns 0  on success and -1 on 
   failure. */
extern int mvrt_sched_run(mvrt_sched_t *sched);
extern int mvrt_sched_stop(mvrt_sched_t *sched);

#endif /* MVRT_SCHED_H */
