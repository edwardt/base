/**
 * @file event.h
 *
 * @brief Interface to events.
 */
#ifndef MV_EVENT_H
#define MV_EVENT_H

#include <mv/defs.h>       /* mv_ptr_t */


/* Opaque pointer to an event. Note that this is just a handle
   to the event which resides inside the runtime. */
typedef mv_ptr_t mv_event_t;

#define MV_EVENT_UNREGISTERD(v) ((v) == 0)

/* Get the handle of the event with the given name. The event should 
   have been already registered. Returns 0 if no such event exists. */
mv_event_t mv_event_handle(mv_value_t str);

/* Generate an instance of the event with optional payload. */
int mv_event_trigger(mv_event_t ev, mv_value_t value);

/* Blocking wait for an instance of an event. On event occurrence, value 
   is filled up with payload. */
int mv_event_wait(mv_event_t ev, mv_value_t& value);


#endif /* MV_EVENT_H */
