/**
 * @file rtevent.h
 *
 * @brief Interface to runtime events and timers.
 */
#ifndef MVRT_EVENT_H
#define MVRT_EVENT_H

#include <stdlib.h>       /* size_t */
#include <common/defs.h>  /* mv_uint32_t */
#include <mv/value.h>     /* mv_value_t */


/* Opaque handle to the event type. */
typedef void mvrt_event_t;

/* Event instance. */
typedef struct mvrt_eventinst {
  mvrt_event_t *type;        /* event */
  mv_value_t data;           /* event payload */
} mvrt_eventinst_t;


/*
 * Functions for events.
 */ 

/* Creates a new event type in a device. When dev is NULL, the event
   is  local event. When the dev is non-NULL, it is a proxy to the
   remote event. A non-NULL dev must not be the name of the device which
   runs the runtime. */
extern mvrt_event_t *mvrt_event_new(const char *name, const char *dev);

/* Creates a local timer event with the interval in secs and nanosecs. */
extern mvrt_event_t *mvrt_timer_new(const char *name, size_t s, size_t ns);

/* Parses a string into an event. The argument string must contain exactly
   one definition of a local event or a local timer. */
extern mvrt_event_t *mvrt_event_load_str(char *line);

/* Prints an event into a string which can be saved to a file. */
extern char *mvrt_event_save_str(mvrt_event_t *ev);

/* Deletes an event from the runtime memory. If the event is local, it is 
   also removed from the local runtime registry. */
extern int mvrt_event_delete(mvrt_event_t *ev);

/* Finds the handle to the given event in a given device. */
extern mvrt_event_t *mvrt_event_lookup(const char *name, const char *dev);


/*
 * Functions for timers.
 */
extern int mvrt_timer_module_init();

/* Starts and stops the timer event. */
extern int mvrt_timer_start(mvrt_event_t *ev);
extern int mvrt_timer_stop(mvrt_event_t *ev);


/*
 * Functions for event instances.
 */
extern mvrt_eventinst_t *mvrt_eventinst_new(mvrt_event_t *ev, mv_value_t v);
extern int mvrt_eventinst_delete(mvrt_eventinst_t *ev);

#endif /* MVRT_EVENT_H */
