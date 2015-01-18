/**
 * @file value.h
 *
 * @brief Interface to values.
 */
#ifndef MV_VALUE_H
#define MV_VALUE_H

#include <mv/defs.h>       /* mv_ptr_t */


/* opaque pointer to value */
typedef mv_ptr_t mv_value_t;

typedef enum {
  MV_VALUE_INVALID = 0x0,  /* invalid value */
  MV_VALUE_NULL    = 0x1,  /* null */
  MV_VALUE_INT     = 0x2,  /* signed integer */
  MV_VALUE_FLOAT   = 0x3,  /* floating number */
  MV_VALUE_STRING  = 0x4,  /* string */
  MV_VALUE_PAIR    = 0x5,  /* pair */
  MV_VALUE_CONS    = 0x6,  /* cons */
  MV_VALUE_MAP     = 0x7,  /* map of (name, value) pairs */
  MV_VALUE_NTAGS
} mv_valuetag_t;

/* common functions for all value types */
mv_valuetag_t mv_value_tag(mv_value_t value);
int mv_value_eq(mv_value_t u, mv_value_t v);

int mv_value_delete(mv_value_t v);
int mv_value_print(mv_value_t v);

/* Transforms values to JSON format and vice versa. For mv_value_to_str,
   the caller is responsible for freeing the returned string. */
char *mv_value_to_str(mv_value_t v);
mv_value_t mv_value_from_str(const char *s);

/* initial (invalid) value */
extern mv_value_t mv_value_init();

/* null */
extern mv_value_t mv_value_null();
extern int mv_value_is_null();

/* integer */
extern mv_value_t mv_value_int(int v);
extern int mv_value_int_get(mv_value_t value);

/* real */
extern mv_value_t mv_value_float(float v);
extern float mv_value_float_get();

/* string */
extern mv_value_t mv_value_string(const char *v);
extern char *mv_value_string_get();

/* pair */
extern mv_value_t mv_value_pair(mv_value_t first, mv_value_t second);
extern mv_value_t mv_value_pair_first(mv_value_t pv);
extern mv_value_t mv_value_pair_second(mv_value_t pv);

/* cons cells */
extern mv_value_t mv_value_cons();
extern mv_value_t mv_value_cons_car(mv_value_t cons);
extern mv_value_t mv_value_cons_cdr(mv_value_t cons);
extern mv_value_t mv_value_cons_setcar(mv_value_t cons, mv_value_t v);
extern mv_value_t mv_value_cons_setcdr(mv_value_t cons, mv_value_t v);

/* key-value map */
extern mv_value_t mv_value_map();
extern mv_value_t mv_value_map_lookup(mv_value_t m, mv_value_t k);
extern mv_value_t mv_value_map_add(mv_value_t m, mv_value_t k, mv_value_t v);

#endif /* MV_VALUE_H */
