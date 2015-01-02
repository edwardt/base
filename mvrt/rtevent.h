/**
 * @file rtevent.h
 *
 * @brief Interface to runtime events. Note that mv/event.h is for 
 * global events, while rtevent is a more general event which also
 * includes local and system events.
 */
#ifndef MVRT_EVENT_H
#define MVRT_EVENT_H

#include <common/defs.h>  /* mv_uint32_t */

typedef enum {
  MVRT_EVENT_SYSTEM,      /* special system event */
  MVRT_EVENT_LOCAL,       /* local events */
  MVRT_EVENT_GLOBAL,      /* remote events */
  MVRT_EVENT_TIMER,       /* local timer events */
  MVRT_EVENT_NTAGS
} mvrt_eventag_t;

/* Opaque handle to the event. */
typedef mv_ptr_t mvrt_event_t;

extern int mvrt_event_module_init();

extern mvrt_event_t mvrt_event_new(const char *dev, const char *ev, int tag);
extern int mvrt_event_delete(mvrt_event_t ev);

/* Create a timer event with the given interval in miliseconds. */
extern mvrt_event_t mvrt_timer_new(const char *name, size_t msec);

extern mvrt_event_t mvrt_event_lookup(const char *dev, const char *name);

extern const char *mvrt_event_dev(mvrt_event_t ev);
extern const char *mvrt_event_name(mvrt_event_t ev);
extern mvrt_eventag_t mvrt_event_tag(mvrt_event_t ev);

extern int mvrt_timer_start(mvrt_event_t ev);
extern int mvrt_timer_stop(mvrt_event_t ev);

#endif /* MVRT_EVENT_H */
