/**
 * @file rtobj.h
 *
 * @brief Interface to the local runtime objects, such as properties, events, 
 * functions, and reactors.
 */
#ifndef MVRT_OBJ_H
#define MVRT_OBJ_H

#include <mv/defs.h>         /* mv_uint32_t */


/* An MV runtime object is a property, an event, a function, or a reactor. */
#define MVRT_OBJ_PROP      0
#define MVRT_OBJ_FUNC      1
#define MVRT_OBJ_EVENT     2
#define MVRT_OBJ_REACTOR   3
#define MVRT_OBJ_NTAGS     4

typedef struct mvrt_obj {
  char *dev;                 /* name of device: NULL for local objects */
  char *name;                /* name of the object */
  mv_uint32_t hash;          /* hash computed from dev and name */

  unsigned tag   : 3;        /* MVRT_OBJ_EVENT, etc. */
  unsigned used  : 1;        /* need to be saved */
  unsigned sys   : 1;        /* system object: NOT used for now */
  unsigned pad   : 28;       /* pad */

  void *data;                /* object-specific data */
} mvrt_obj_t;


/* Initializes the MVRT obj item table. Call at time 0. */
extern int mvrt_obj_module_init();

/* Creates a new runtime object. Caller is responsible for filling in the 
   fields except for the name. Returns -1 when 1) there is a name conflict 
   or 2) the maximum number supported runtime objects is reached. 

  NOTE: Do not directly call this. Call mvrt_prop_new or mvrt_event_new
  instead. */
extern mvrt_obj_t *mvrt_obj_new(const char *name, const char *dev);

/* Removes the given object from the local registry of runtime objects. 

  NOTE: Do not directly call this. Call mvrt_prop_delete or mvrt_event_delete
  instead. */
extern int mvrt_obj_delete(mvrt_obj_t *o);

/* Looks up the runtime object. Returns NULL if no such object eixsts. */
extern mvrt_obj_t *mvrt_obj_lookup(const char *name, const char *dev);

/* Loads runtime objects from the given file. */
extern int mvrt_obj_loadfile(const char *file);

/* Saves runtime objects to the given file. */
extern int mvrt_obj_savefile(const char *file);

#endif /* MVRT_OBJ_H */
