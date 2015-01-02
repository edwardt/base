/**
 * @file event.h
 *
 * @brief Interface to event service, which provides a space of events
 * across the set of all devices. Eventually, we need to implement this
 * layer in a distributed way.
 */
#ifndef MV_EVENT_H
#define MV_EVENT_H

#include <mv/value.h>   /* mv_value_t */

#ifdef __cplusplus
extern "C" {
#endif

/* Initializes the event service. Be sure to call this at time 0. */
extern int mv_event_service_init();

/* Registers or deregisters an event. The reason why we need registration
   for events (even when events are per-device entity) is that maintaining
   event subscription information in the server might be beneficial for 
   scaling purpose. 

   TODO: not implemented yet.

   TODO: For now, event is owned by a device and only one device can
   publish the given event. Later, we could support the notion of "topic",
   i.e. an event, which can be published by multiple publisher devices. 

   Q: Do we really need to maintain subscription info on server? */
/*
  extern int mv_event_register(const char *dev, const char *ev);
  extern int mv_event_deregister(const char *dev, const char *ev);
*/

/* Subscribes to the given remote event. That is, add the sub device
   to the list of the subscribes of the given event. */
extern int mv_event_subscribe(const char *dev, const char *ev, const char *sub);

/* Publishs the given local event. All remote subscribers will be sent 
   messages for event occurrences. */
extern int mv_event_publish(const char *dev, const char *ev, mv_value_t val);

#ifdef __cplusplus
}
#endif 

#endif /* MV_EVENT_H */
