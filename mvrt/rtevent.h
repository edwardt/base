/**
 * @file rtevent.h
 *
 * @brief Interface to runtime events. Note that mv/event.h is for 
 * global events, while rtevent is a more general event which also
 * includes local and system events.
 */
#ifndef MVRT_EVENT_H
#define MVRT_EVENT_H

#include <stdlib.h>       /* size_t */
#include <common/defs.h>  /* mv_uint32_t */
#include <mv/value.h>     /* mv_value_t */

typedef enum {
  MVRT_EVENT_SYSTEM,      /* system-defined local event */
  MVRT_EVENT_LOCAL,       /* user-defined local events */
  MVRT_EVENT_TIMER,       /* user-defined local timer events */
  MVRT_EVENT_GLOBAL,      /* remote events */
  MVRT_EVENT_NTAGS
} mvrt_eventag_t;

/* Opaque handle to the event type. */
typedef mv_ptr_t mvrt_event_t;

typedef struct mvrt_eventinst {
  mvrt_event_t type;         /* event type */
  mv_value_t data;           /* event payload */
} mvrt_eventinst_t;

/*
 * Functions for events.
 */ 
extern int mvrt_event_module_init();

/* Creates/deletes a new event type in a device. The mvrt_event_t object
   will represent the given event type. */
extern mvrt_event_t mvrt_event_new(const char *dev, const char *ev, int tag);
extern int mvrt_event_delete(mvrt_event_t ev);

/* Finds the handle to the given event in a given device. */
extern mvrt_event_t mvrt_event_lookup(const char *dev, const char *name);

/* Queries the details of an event. */
extern const char *mvrt_event_dev(mvrt_event_t ev);
extern const char *mvrt_event_name(mvrt_event_t ev);
extern mvrt_eventag_t mvrt_event_tag(mvrt_event_t ev);

/*
 * Functions for timers.
 */
extern int mvrt_timer_module_init();

/* Creates a timer event with the interval in seconds and nanoseconds. */
extern mvrt_event_t mvrt_timer_new(const char *name, size_t s, size_t ns);

/* Starts and stops the timer event. */
extern int mvrt_timer_start(mvrt_event_t ev);
extern int mvrt_timer_stop(mvrt_event_t ev);

/*
 * Functions for event instances.
 */
extern mvrt_eventinst_t *mvrt_eventinst_new(mvrt_event_t ev, mv_value_t data);
extern int mvrt_eventinst_delete(mvrt_eventinst_t *ev);

#endif /* MVRT_EVENT_H */
