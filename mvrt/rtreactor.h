/**
 * @file rtreactor.h
 *
 * @brief Interface to reactors.
 */
#ifndef MVRT_REACTOR_H
#define MVRT_REACTOR_H

#include <stdio.h>          /* FILE */
#include <common/defs.h>    /* mv_uint32_t */
#include "rtevent.h"        /* mvrt_event_t */
#include "rtcode.h"         /* mvrt_code_t */


/* Opaque pointer to reactors. */
typedef void mvrt_reactor_t;

/* Reactor list. */
typedef struct mvrt_reactor_list {
  mvrt_reactor_t *reactor;
  struct mvrt_reactor_list *next;
} mvrt_reactor_list_t;


/*
 * Functions for reactors.
 */

/* Creates a newr reactor. */
extern mvrt_reactor_t *mvrt_reactor_new(const char *name);

/* Parses a file starting from the given line, into a reactor. The file 
   position in fp is advanced to the end of a reactor definition. */
extern mvrt_reactor_t *mvrt_reactor_load_str(char *line, FILE *fp);

/* Prints a reactor into a string which can be saved to a file. */
extern char *mvrt_reactor_save_str(mvrt_reactor_t *reactor);

extern int mvrt_reactor_delete(mvrt_reactor_t *reactor);

extern mvrt_code_t *mvrt_reactor_getcode(mvrt_reactor_t *reactor);
extern int mvrt_reactor_addcode(mvrt_reactor_t *reactor, mvrt_code_t *code);

extern mvrt_reactor_t *mvrt_reactor_lookup(const char *name);


/*
 * Functions for event-reactor associations.
 */
extern mvrt_reactor_t *mvrt_reactor_assoc_load_str(char *str);

extern int mvrt_add_reactor_to_event(mvrt_event_t *ev, mvrt_reactor_t *r);
extern int mvrt_remove_reactor_from_event(mvrt_event_t *ev, mvrt_reactor_t *r);

extern mvrt_reactor_list_t *mvrt_get_reactors_for_event(mvrt_event_t *ev);


#endif /* MVRT_REACTOR_H */
