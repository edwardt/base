/**
 * @file rtvalue.c
 */
#include <stdio.h>     /* fprintf */
#include <stdlib.h>    /* malloc */
#include <string.h>    /* strdup */
#include <assert.h>    /* assert */
#include <malloc.h>    /* memalign */
#include "rtvalue.h"
#include "rtfunc.h"
#include "rtevent.h"


/*
 * Functions for the value API.
 */
mvrt_valuetag_t mvrt_value_tag(mv_value_t v)
{
  return MVRT_VALUE_TAG(v);
}

int mvrt_value_eq(mvrt_value_t u, mvrt_value_t v)
{
  return mv_value_eq((mv_value_t) u, (mv_value_t) v);
}

int mvrt_value_delete(mvrt_value_t value)
{
  return 0;
}


int mvrt_value_print(mvrt_value_t value)
{
  const char *str = mvrt_value_to_str(value);
  if (!str)
    return -1;

  fprintf(stdout, "%s\n", str);

  return 0;
}

const char *mvrt_value_to_str(mvrt_value_t value)
{
  static char buf[4096];

  mvrt_funcval_t *rtfunc_v;
  mvrt_eventval_t *rtev_v;
  mvrt_func_t rtfunc;
  mvrt_event_t rtev;
  int tag;
  switch (MVRT_VALUE_TAG(value)) {
  case MVRT_VALUE_FUNC:
    rtfunc_v = (mvrt_funcval_t *) MVRT_VALUE_PTR(value);
    rtfunc = rtfunc_v->func;
    tag = mvrt_func_tag(rtfunc);
    if (tag == MVRT_FUNC_NATIVE) {
      mvrt_native_t *native = mvrt_func_native(rtfunc);
      sprintf(buf, "F[%s/%s]\n", native->lib, native->name);
    }
    else if (tag == MVRT_FUNC_GLOBAL)
      sprintf(buf, "F[%s:%s]\n", mvrt_func_dev(rtfunc), mvrt_func_name(rtfunc));
    else
      sprintf(buf, "F[%s]\n", mvrt_func_name(rtfunc));
    return &buf[0];
    break;
  case MVRT_VALUE_EVENT:
    rtev_v = (mvrt_eventval_t *) MVRT_VALUE_PTR(value);
    rtev = rtev_v->type;
    sprintf(buf, "E[%s]\n", mvrt_event_name(rtev));
    return &buf[0];
  default:
    return mv_value_to_str((mv_value_t) value);
  }
  return NULL;
}

static void *_null = NULL;
mvrt_value_t mvrt_value_null()
{
  return (mvrt_value_t) mv_value_null();
}

int mvrt_value_is_null(mvrt_value_t v)
{
  return mv_value_is_null(v);
}

mvrt_value_t mvrt_value_int(int v)
{
  return (mvrt_value_t) mv_value_int(v);
}

int mvrt_value_int_get(mvrt_value_t v) 
{
  return mv_value_int_get((mv_value_t) v);
}

mvrt_value_t mvrt_value_float(float v)
{
  return (mvrt_value_t) mv_value_float(v);
}

float mvrt_value_float_get(mvrt_value_t v) 
{
  return mv_value_float_get((mv_value_t) v);
}

mvrt_value_t mvrt_value_string(const char *v)
{
  return (mvrt_value_t) mv_value_string(v);
}

char *mvrt_value_string_get(mvrt_value_t v) 
{
  return mv_value_string_get((mv_value_t) v);
}

mvrt_value_t mvrt_value_event(mvrt_event_t e, mvrt_value_t v)
{
  /* mvrt_eventval_t *ev = malloc(sizeof(mvrt_eventval_t)); */
  mvrt_eventval_t *ev = memalign(16, sizeof(mvrt_eventval_t));
  ev->type = e;
  ev->data = v;

  return MVRT_VALUE_TAGPTR(ev, MVRT_VALUE_EVENT);
}

mvrt_event_t mvrt_value_event_type(mvrt_value_t v)
{
  assert(MVRT_VALUE_TAG(v) == MVRT_VALUE_EVENT);
  mvrt_eventval_t *ev = (mvrt_eventval_t *) MVRT_VALUE_PTR(v);

  return ev->type;
}

mvrt_value_t mvrt_value_event_data(mvrt_value_t v)
{
  assert(MVRT_VALUE_TAG(v) == MVRT_VALUE_EVENT);
  mvrt_eventval_t *ev = (mvrt_eventval_t *) MVRT_VALUE_PTR(v);

  return ev->data;
}

mvrt_value_t mvrt_value_pair(mvrt_value_t first, mvrt_value_t second)
{
  return mv_value_pair((mv_value_t) first, (mv_value_t) second);
}

mvrt_value_t mvrt_value_pair_first(mvrt_value_t pv)
{
  return (mvrt_value_t) mv_value_pair_first((mv_value_t) pv);
}

mvrt_value_t mvrt_value_pair_second(mvrt_value_t pv)
{
  return (mvrt_value_t) mv_value_pair_second((mv_value_t) pv);
}

mvrt_value_t mvrt_value_cons()
{
  return (mvrt_value_t) mv_value_cons();
}

mvrt_value_t mvrt_value_cons_car(mvrt_value_t v)
{
  return (mvrt_value_t) mv_value_cons_car((mv_value_t) v);
}

mvrt_value_t mvrt_value_cons_cdr(mvrt_value_t v)
{
  return (mvrt_value_t) mv_value_cons_cdr((mv_value_t) v);
}

mvrt_value_t mvrt_value_cons_setcar(mvrt_value_t cv, mvrt_value_t v)
{
  return (mvrt_value_t) mv_value_cons_setcar((mv_value_t) cv, (mv_value_t) v);
}

mvrt_value_t mvrt_value_cons_setcdr(mvrt_value_t cv, mvrt_value_t v)
{
  return (mvrt_value_t) mv_value_cons_setcdr((mv_value_t) cv, (mv_value_t) v);
}

mvrt_value_t mvrt_value_map()
{
  return (mvrt_value_t) mv_value_map();
}

mvrt_value_t mvrt_value_map_lookup(mvrt_value_t mv, mvrt_value_t k)
{
  return (mvrt_value_t) mv_value_map_lookup((mv_value_t) mv, (mv_value_t) k);
}

mvrt_value_t mvrt_value_map_add(mvrt_value_t mv, mvrt_value_t k, mvrt_value_t v)
{
  return (mvrt_value_t) mv_value_map_add((mv_value_t) mv, (mv_value_t) k, 
                                         (mv_value_t) v);
}

mvrt_value_t mvrt_value_func(mvrt_func_t f)
{
  /* mvrt_funcval_t *fun = malloc(sizeof(mvrt_funcval_t)); */
  mvrt_funcval_t *fun = memalign(16, sizeof(mvrt_funcval_t));
  fun->func = f;
  fun->env = mvrt_value_null();

  return MVRT_VALUE_TAGPTR(fun, MVRT_VALUE_FUNC);
}

mvrt_func_t mvrt_value_func_get(mvrt_value_t f)
{
  assert(MVRT_VALUE_TAG(f) == MVRT_VALUE_FUNC);
  mvrt_funcval_t *fun = (mvrt_funcval_t *) MVRT_VALUE_PTR(f);

  return fun->func;
}

