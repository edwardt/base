/**
 * @file reactor.h
 *
 * @brief Interface to reactors.
 */
#ifndef MVRT_REACTOR_H
#define MVRT_REACTOR_H

#include <common/defs.h>    /* mv_uint32_t */
#include "rtevent.h"        /* mvrt_event_t */
#include "code.h"           /* mvrt_code_t */

typedef mv_uint32_t mvrt_reactorid_t;
typedef struct mvrt_reactor {
  mvrt_reactorid_t id;
  char *name;               /* optional: name of reactor */

  mvrt_code_t *code;        /* code */
} mvrt_reactor_t;

typedef struct mvrt_reactor_list {
  mvrt_reactor_t *reactor;
  struct mvrt_reactor_list *next;
} mvrt_reactor_list_t;


/* Initializes the event module. Be sure to call this at time 0. */
extern int mvrt_reactor_module_init();

extern int mvrt_reactor_loadfile(const char *file);
extern int mvrt_reactor_assoc_loadfile(const char *file);

extern mvrt_reactor_t *mvrt_reactor(const char *name);
extern int mvrt_reactor_delete(mvrt_reactor_t *react);
extern mvrt_reactor_t *mvrt_reactor_lookup(const char *name);

/* event-reactor table */
extern int mvrt_add_reactor_to_event(mvrt_event_t ev, mvrt_reactor_t *r);
extern int mvrt_remove_reactor_from_event(mvrt_event_t ev, mvrt_reactor_t *r);
extern mvrt_reactor_list_t *mvrt_get_reactors_for_event(mvrt_event_t ev);

#endif /* MVRT_REACTOR_H */
