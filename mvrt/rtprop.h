/**
 * @file rtprop.h
 *
 * @brief Interface to runtime properites. Note that mv/prop.h is for 
 * global properties, while rtprop is a more general property which also
 * includes local and system properties.
 */
#ifndef MVRT_PROP_H
#define MVRT_PROP_H

#include <common/defs.h>   /* mv_ptr_t */
#include <mv/value.h>      /* mv_value_t */


/* Opaque handle for the property structure. */
typedef mv_ptr_t mvrt_prop_t;

typedef enum {
  MVRT_PROP_SYSTEM  = 0x1,
  MVRT_PROP_LOCAL   = 0x2,
  MVRT_PROP_GLOBAL  = 0x4,
  MVRT_PROP_NTAGS
} mvrt_proptag_t;

/* Initializes the property module. Be sure to call at time 0. */
extern int mvrt_prop_module_init();

/* Load or save properties using the given file. */
extern int mvrt_prop_loadfile(const char *file, mvrt_proptag_t tag);
extern int mvrt_prop_savefile(const char *file, mvrt_proptag_t tag);

extern mvrt_prop_t mvrt_prop_new(const char *dev, const char *name, int tag);
extern int mvrt_prop_delete(mvrt_prop_t prop);

extern mvrt_prop_t mvrt_prop_lookup(const char *dev, const char *name);

extern const char *mvrt_prop_dev(mvrt_prop_t prop);
extern const char *mvrt_prop_name(mvrt_prop_t prop);
extern mvrt_proptag_t mvrt_prop_tag(mvrt_prop_t prop);

extern mv_value_t mvrt_prop_getvalue(mvrt_prop_t prop);
extern int mvrt_prop_setvalue(mvrt_prop_t prop, mv_value_t value);

#endif /* MVRT_PROP_H */
