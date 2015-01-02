/**
 * @file rtvalue.h
 *
 * @brief Interface to runtime values.
 */
#ifndef MVRT_VALUE_H
#define MVRT_VALUE_H

#include <common/defs.h>   /* mv_ptr_t */
#include <mv/value.h>      /* mv_value_t */
#include "rtevent.h"       /* mvrt_event_t */

typedef mv_ptr_t mvrt_func_t;

/* Extends mv_valuetag_t defined in mv/value.h.
 
   TODO: Currently, there are 9 value tags, which mandates that any 
   mv(rt)_value_t structs to be 16-byte-aligned (128-bit). Can we consolidate 
   number of value tags to 8 so that we can all value structs to be 8-byte 
   aligned (multiples of 64-bit)? 
*/
typedef enum {
  MVRT_VALUE_EVENT = 0x7,
  MVRT_VALUE_FUNC  = 0x8,
  MVRT_VALUE_NTAGS
} mvrt_valuetag_t;

typedef mv_ptr_t mvrt_value_t;

/* FIXME: mvrt_eventval and mvrt_funcval must be allocated in 16-byte 
   aligned way to allow tag to be stored in 4 LSB bits. */
typedef struct mvrt_eventval {
  mvrt_event_t type;         /* event */
  mvrt_value_t data;         /* payload */
  
  mvrt_value_t pada;
  mvrt_value_t padb;
} mvrt_eventval_t;

typedef struct mvrt_funcval {
  mvrt_func_t func;          /* function */
  mvrt_value_t env;          /* map of (name, value) bindings */
} mvrt_funcval_t;

#define MVRT_VALUE_TAG(val) ((val) & 0xf)
#define MVRT_VALUE_PTR(val) ((void *) ((val) & ~((mvrt_value_t) 0xf)))
#define MVRT_VALUE_TAGPTR(ptr, tag) ((mvrt_value_t) (ptr) | (tag))

mvrt_valuetag_t mvrt_value_tag(mvrt_value_t v);

int mvrt_value_eq(mvrt_value_t u, mvrt_value_t v);
int mvrt_value_delete(mvrt_value_t v);

int mvrt_value_print(mvrt_value_t v);

/* Returns the string in JSON format. Caller is reponsible for freeing 
   the returned string. */
const char *mvrt_value_to_str(mvrt_value_t v);


/* null */
extern mvrt_value_t mvrt_value_null();
extern int mvrt_value_is_null(mvrt_value_t value);

/* integer */
extern mvrt_value_t mvrt_value_int(int v);
extern int mvrt_value_int_get(mvrt_value_t value);

/* real */
extern mvrt_value_t mvrt_value_float(float v);
extern float mvrt_value_float_get(mvrt_value_t value);

/* string */
extern mvrt_value_t mvrt_value_string(const char *v);
extern char *mvrt_value_string_get(mvrt_value_t value);

/* event instances */
extern mvrt_value_t mvrt_value_event(mvrt_event_t ev, mvrt_value_t v);
extern mvrt_value_t mvrt_value_event_data(mvrt_value_t v);
extern mvrt_event_t mvrt_value_event_type(mvrt_value_t v);

extern mvrt_value_t mvrt_value_pair(mvrt_value_t first, mvrt_value_t second);
extern mvrt_value_t mvrt_value_pair_first(mvrt_value_t pair);
extern mvrt_value_t mvrt_value_pair_second(mvrt_value_t pair);

/* cons cells */
extern mvrt_value_t mvrt_value_cons();
extern mvrt_value_t mvrt_value_cons_car(mvrt_value_t cons);
extern mvrt_value_t mvrt_value_cons_cdr(mvrt_value_t cons);
extern mvrt_value_t mvrt_value_cons_setcar(mvrt_value_t cons, mvrt_value_t v);
extern mvrt_value_t mvrt_value_cons_setcdr(mvrt_value_t cons, mvrt_value_t v);

/* dictionaries */
extern mvrt_value_t mvrt_value_map();
extern mvrt_value_t mvrt_value_map_lookup(mvrt_value_t map, mvrt_value_t key);
extern mvrt_value_t mvrt_value_map_add(mvrt_value_t mp, mvrt_value_t k, 
                                       mvrt_value_t v);

/* functions */
extern mvrt_value_t mvrt_value_func(mvrt_func_t func);
extern mvrt_func_t mvrt_value_func_get(mvrt_value_t f);

#endif /* MVRT_VALUE_H */
