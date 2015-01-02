/**
 * @file mdecoder.h
 *
 * @brief Interface to message decoder.
 */
#ifndef MVRT_MDECODER_H
#define MVRT_MDECODER_H

#include <mq/mqueue.h>    /* mv_mqueue_t */
#include "evqueue.h"

typedef void mvrt_mdecoder_t;

/* Create a decoder from the given message queue, which will decode a 
   string from the queue, create a MV message, and eventually create
   an event into the event queue. */
extern mvrt_mdecoder_t *mvrt_mdecoder(mv_mqueue_t *mq, mvrt_evqueue_t *evq);
extern int mvrt_mdecoder_run(mvrt_mdecoder_t *mf);

#endif /* MVRT_MDECODER_H */
