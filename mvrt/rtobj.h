/**q
 * @file rtobj.h
 *
 * @brief Interface to the runtime objects, such as properties, events, 
 * functions, and reactors.
 */
#ifndef MVRT_OBJ_H
#define MVRT_OBJ_H

#include <common/defs.h>
#include "rtprop.h"
#include "rtfunc.h"
#include "rtevent.h"
#include "rtreactor.h"

/* An MV runtime object is a property, an event, a function, or a reactor. */
#define MVRT_OBJ_EVENT     0x0
#define MVRT_OBJ_FUNC      0x1
#define MVRT_OBJ_PROP      0x2
#define MVRT_OBJ_REACTOR   0x3
#define MVRT_OBJ_NTAGS     0x4

/* An MV runtime object can be either local or external. */
#define MVRT_OBJ_LOCAL     0x0
#define MVRT_OBJ_EXTERN    0x1

typedef struct mvrt_obj {
  char *dev;                 /* device which contains this object */
  char *name;                /* name of the object */
  mv_uint32_t hash;          /* hash computed from dev and name */

  mv_uint32_t tag   : 2;     /* MVRT_OBJ_EVENT, etc. */
  mv_uint32_t loc   : 1;     /* MVRT_OBJ_LOCAL, etc */
  mv_uint32_t used  : 1;     /* need to be saved */
  mv_uint32_t pad   : 28;    /* pad */

  union {
    mvrt_prop_t prop;        /* prop */
    mvrt_func_t func;        /* func */
    mvrt_event_t event;      /* event */
    mvrt_reactor_t reactor;  /* reactor */
  } u;
} mvrt_obj_t;


/* Initializes the MVRT obj item table. Call at time 0. */
extern int mvrt_obj_module_init();

/* Creates a new runtime object. Caller is responsible for filling in the 
   fields except for dev and name. Returns -1 when 1) there is a name
   conflict or 2) the maximum number supported runtime objects is reached. */
extern mvrt_obj_t *mvrt_obj_new(const char *dev, const char *name);

/* Removes the given object the table. If the obj is local, it
   is permanently removed. If the obj is extern, only the local proxy
   for the obj item is removed. */
extern int mvrt_obj_delete(mvrt_obj_t *obj);

/* Looks up the runtime object. If dev is NULL or ":", it only searches for
   a local obj item. Returns NULL if no such object eixsts*/
extern mvrt_obj_t *mvrt_obj_lookup(const char *dev, const char *name);

/* Loads or saves runtime objects using the given file. */
extern int mvrt_obj_loadfile(const char *file);
extern int mvrt_obj_savefile(const char *file);

#endif /* MVRT_OBJ_H */
