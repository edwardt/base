/**
 * @file service.h
 *
 * @brief A device can provide services to other devices. Also, a device
 * can use services provided by other devices. 
 */
#ifndef MV_SERVICE_H
#define MV_SERVICE_H

#include <value.h>


/* Opaque pointer to a service. An mv_stag_t is a pointer value where 
   two lsb bits are used for storing a tag. */
typedef mv_ptr_t mv_service_t;

typedef enum {
  MV_SERVICE_PROP  = 0x0,   /* property */
  MV_SERVICE_FUNC  = 0x1,   /* function */
  MV_SERVICE_EVENT = 0x2,   /* event */
  MV_SERVICE_NTAGS
} mv_stag_t;

#define MV_SERVICE_INVALID(v) ((v) == 0)

/*
 * Common functions for all service types.
 */
mv_stag_t mv_service_tag(mv_service_t s);
int mv_service_eq(mv_service_t s, mv_service_t t);

int mv_service_delete(mv_service_t s);
int mv_service_print(mv_service_t s);

/*
 * Properties
 */
/* Returns the value of property @p. Returns the INVALID value on failure. */
mv_value_t mv_prop_get(mv_prop_t p);

/* Sets the value of a property @p. Returns 0 on success, otherwise -1. */
int mv_prop_set(mv_prop_t p, mv_value_t v);

/*
 * For functions.
 */
mv_value_t mv_func_call(mv_func_t f, mv_value_t args);


/*
 * For events.
 */
/* Get the handle of the event with the given name. The event should 
   have been already registered. Returns 0 if no such event exists. */
mv_event_t mv_event_handle(mv_value_t str);

/* Generate an instance of the event with optional payload. */
int mv_event_trigger(mv_event_t ev, mv_value_t value);

/* Blocking wait for an instance of an event. On event occurrence, value 
   is filled up with payload. */
int mv_event_wait(mv_event_t ev, mv_value_t& value);




#endif /* MV_SERVICE_H */
