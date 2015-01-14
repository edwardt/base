/**
 * @file rtfunc.h
 *
 * @brief Interface to runtime functions.
 */
#ifndef MVRT_FUNC_H
#define MVRT_FUNC_H

#include <common/defs.h>   /* mv_prt_t */
#include "code.h"          /* mvrt_code_t */


typedef int (*mvrt_native_func1_t)(int);
typedef void *(*mvrt_native_func2_t)(int, int);
typedef struct mv_native {
  char *lib;                  /* dynamic library name */
  char *name;                 /* function name */
  mvrt_native_func1_t func1;  /* native function */
  mvrt_native_func2_t func2;  /* native function */
} mvrt_native_t;

/* Opaque handle to the runtime function. */
typedef void mvrt_func_t;


/* Creates a new local function. */
extern mvrt_func_t *mvrt_func_new(const char *name);

/* Parses a file starting from the given line, into a function. The file 
   position in fp is advanced to the end of a reactor definition. */
extern mvrt_func_t *mvrt_func_load_str(char *line, FILE *fp);

/* Prints a reactor into a string which can be saved to a file. */
extern char *mvrt_func_save_str(mvrt_func_t *func);

/* Deletes a local function. */
extern int mvrt_func_delete(mvrt_func_t *func);

/* Looks up a local function. */
extern mvrt_func_t *mvrt_func_lookup(const char *name);

/* Returns 1 if the function is a native function. Returns 0 otherwise. */
extern int mvrt_func_isnative(mvrt_func_t *func);

/* Returns the given native function. */
extern mvrt_native_t *mvrt_func_getnative(mvrt_func_t *func);

#endif /* MVRT_FUNC_H */
