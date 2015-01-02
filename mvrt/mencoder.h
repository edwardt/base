/**
 * @file mencoder.h
 *
 * @brief Interface to message encoder.
 */
#ifndef MV_MENCODER_H
#define MV_MENCODER_H

#include "mqueue.h"

typedef void mv_mencoder_t;

/* Create a encoder from the given message queue, which will decode a 
   string from the queue, create a MV message, and eventually create
   an event into the event queue. */
extern mv_mencoder_t *mv_mencoder(mv_mqueue_t *mq);
extern int mv_mencoder_run(mv_mencoder_t *mf);

extern int mv_mencoder_send(mv_message_t *msg);

#endif /* MV_MENCODER_H */
