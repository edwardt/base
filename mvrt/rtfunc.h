/**
 * @file rtfunc.h
 *
 * @brief Interface to runtime functions.
 */
#ifndef MVRT_FUNC_H
#define MVRT_FUNC_H

#include <common/defs.h>   /* mv_prt_t */
#include "code.h"          /* mvrt_code_t */

typedef enum {
  MVRT_FUNC_SYSTEM,        /* sytem function */
  MVRT_FUNC_LOCAL,         /* user-defined MV function */
  MVRT_FUNC_NATIVE,        /* user-defined native function */
  MVRT_FUNC_GLOBAL,        /* remote function */
  MVRT_FUNC_NTAGS
} mvrt_functag_t;


typedef int (*mvrt_native_func1_t)(int);
typedef void *(*mvrt_native_func2_t)(int, int);
typedef struct mv_native {
  char *lib;                  /* dynamic library name */
  char *name;                 /* function name */
  mvrt_native_func1_t func1;  /* native function */
  mvrt_native_func2_t func2;  /* native function */
} mvrt_native_t;

/* Opaque handle to the runtime function. */
typedef mv_ptr_t mvrt_func_t;

/* Call this at time 0. */
extern int mvrt_func_module_init();

extern int mvrt_func_loadfile(const char *file, mvrt_functag_t tag);
extern int mvrt_func_savefile(const char *file, mvrt_functag_t tag);

extern mvrt_func_t mvrt_func_new(const char *dev, const char *name, int tag);
extern int mvrt_func_delete(mvrt_func_t func);

extern mvrt_func_t mvrt_func_lookup(const char *dev, const char *name);

extern const char *mvrt_func_dev(mvrt_func_t func);
extern const char *mvrt_func_name(mvrt_func_t func);
extern mvrt_functag_t mvrt_func_tag(mvrt_func_t func);
extern mvrt_native_t *mvrt_func_native(mvrt_func_t func);

typedef mv_ptr_t mvrt_value_t;
extern mvrt_value_t mvrt_func_call(mvrt_func_t func, mvrt_value_t args);


#endif /* MVRT_FUNC_H */
