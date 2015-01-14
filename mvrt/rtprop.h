/**
 * @file rtprop.h
 *
 * @brief Interface to local properties. 
 */
#ifndef MVRT_PROP_H
#define MVRT_PROP_H

#include <common/defs.h>   /* mv_ptr_t */
#include <mv/value.h>      /* mv_value_t */

typedef void mvrt_prop_t;

/* Creates a new property struct. The value field will be initialized
   to MV_VALUE_INVALID. */
extern mvrt_prop_t *mvrt_prop_new(const char *name);

/* Parse a string into a property. The argument string must contain exactly
   one definition of a property. */
extern mvrt_prop_t *mvrt_prop_load_str(char *line);

/* Prints a property into a string which can be saved to a file. */
extern char *mvrt_prop_save_str(mvrt_prop_t *p);

/* Deletes a local property. */
extern int mvrt_prop_delete(mvrt_prop_t *p);
extern int mvrt_prop_delete_by_name(const char *name);

/* Looks up a local property.*/
extern mvrt_prop_t *mvrt_prop_lookup(const char *name);

extern int mvrt_prop_is_local(mvrt_prop_t *p);

/* Returns the value of a property. */
extern mv_value_t mvrt_prop_getvalue(mvrt_prop_t *p);
extern mv_value_t mvrt_prop_getvalue_by_name(const char *name);

/* Sets the value of a property. */
extern int mvrt_prop_setvalue(mvrt_prop_t *p, mv_value_t v);
extern int mvrt_prop_setvalue_by_name(const char *name, mv_value_t v);

#endif /* MVRT_PROP_H */
