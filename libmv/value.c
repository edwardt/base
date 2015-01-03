/**
 * @file value.c
 */
#include <stdio.h>     /* fprintf */
#include <stdlib.h>    /* malloc */
#include <string.h>    /* strdup */
#include <assert.h>    /* assert */
#include <mv/value.h>  /* mv_value_t */


#define _VALUE_TAG(val) ((val) & 0x7)
#define _VALUE_PTR(val) ((void *) ((val) & ~((mv_value_t) 0x7)))
#define _VALUE_TAGPTR(ptr, tag) ((mv_value_t) (ptr) | (tag))
#define _VALUE_IS_PRIM(val) ((val) >= MV_VALUE_INT && (val) <= MV_VALUE_STRING)

typedef struct _value _value_t;

typedef struct _prim {
  unsigned ptag : 3;     /* mv_valuetag_t */
  unsigned pad  : 29;
  union {
    int ival;
    float fval;
    char *sval;
  } u;
} _prim_t;

typedef struct _pair {
  mv_value_t first;
  mv_value_t second;
} _pair_t;

typedef struct _cons {
  mv_value_t car;
  mv_value_t cdr;
} _cons_t;

typedef struct _map {
  /* 
     TODO: create a search tree of pairs
     NOTE: to manipulate bindings, use API functions in value.h - do not
           directly access fields
  */
  mv_value_t bindings;  /* cons */
} _map_t;

static int _value_print(mv_value_t v, char *buf, int bufptr, int maxbuf);
static int _value_print_intlen(int n);
static char *_value_to_str(mv_value_t v);

int _value_print_intlen(int n) {
  int len = (n < 0) ? 2 : 1;
  while (n > 9) {
    n /= 10;
    len++;
  }
  return len;
}

int _value_print(mv_value_t v, char *buf, int bufptr, int maxbuf)
{
  _prim_t *prim;       /* prim */
  _map_t *map;         /* map */
  mv_value_t cons;     /* cons */

  if (bufptr > maxbuf) {
    return -1;
  }

  switch (mv_value_tag(v)) {
  case MV_VALUE_NULL:
    sprintf(buf+bufptr, "NULL");
    bufptr += 4;
    break;
  case MV_VALUE_INT:
    prim = (_prim_t *) _VALUE_PTR(v);
    sprintf(buf+bufptr, "%d", prim->u.ival);
    bufptr += _value_print_intlen(prim->u.ival);
    break;
  case MV_VALUE_FLOAT:
    prim = (_prim_t *) _VALUE_PTR(v);
    sprintf(buf+bufptr, "%.2f", prim->u.fval);
    bufptr += (_value_print_intlen((int) prim->u.fval) + 3);
    break;
  case MV_VALUE_STRING:
    prim = (_prim_t *) _VALUE_PTR(v);
    sprintf(buf+bufptr, "\"%s\"", prim->u.sval);
    bufptr += strlen(prim->u.sval) + 2;
    break;
  case MV_VALUE_PAIR:
    bufptr = _value_print(mv_value_pair_first(v), buf, bufptr, maxbuf);
    sprintf(buf+bufptr, ": ");
    bufptr += 2;
    bufptr = _value_print(mv_value_pair_second(v), buf, bufptr, maxbuf);
    break;
  case MV_VALUE_CONS:
    sprintf(buf+bufptr, "[ ");
    bufptr += 2;
    cons = v;
    while (!mv_value_is_null(cons)) {
      bufptr = _value_print(mv_value_cons_car(cons), buf, bufptr, maxbuf);
      cons = mv_value_cons_cdr(cons);
      sprintf(buf+bufptr, " ");
      bufptr += 1;
    }
    sprintf(buf+bufptr, "]");
    bufptr += 1;
    break;
  case MV_VALUE_MAP:
    map = (_map_t *) _VALUE_PTR(v);
    cons = map->bindings;
    sprintf(buf+bufptr, "{ ");
    bufptr += 2;
    while (!mv_value_is_null(cons)) {
      bufptr = _value_print(mv_value_cons_car(cons), buf, bufptr, maxbuf);
      cons = mv_value_cons_cdr(cons);
      if (!mv_value_is_null(cons)) {
        sprintf(buf+bufptr, ", ");
        bufptr += 2;
      }
    }
    sprintf(buf+bufptr, " }");
    bufptr += 2;
    break;
  default:
    break;
  }

  return bufptr;
}

char *_value_to_str(mv_value_t value)
{
  char *buf;        /* buffer */
  int bufsize;      /* size of buffer */
  int maxsize;      /* maximum buffer size */

  bufsize = 1024;
  maxsize = 1 << 16;
  buf = malloc(bufsize);
  do {
    if (_value_print(value, buf, 0, bufsize) != -1)
      break;
    bufsize = bufsize * 2;
    if (bufsize > maxsize)
      return NULL;
    buf = realloc(buf, maxsize);
  } while (1);

  return buf;
}


/*
 * Functions for the value API.
 */
mv_valuetag_t mv_value_tag(mv_value_t v)
{
  return _VALUE_TAG(v);
}

int mv_value_eq(mv_value_t u, mv_value_t v)
{
  mv_valuetag_t utag = _VALUE_TAG(u);
  mv_valuetag_t vtag = _VALUE_TAG(v);
  if (utag != vtag)
    return 0;

  assert(_VALUE_IS_PRIM(utag));
  _prim_t *uprim = (_prim_t *) _VALUE_PTR(u);
  _prim_t *vprim = (_prim_t *) _VALUE_PTR(v);

  switch (uprim->ptag) {
  case MV_VALUE_INT:
    return (uprim->u.ival == vprim->u.ival) ? 1 : 0;
  case MV_VALUE_FLOAT:
    return (uprim->u.fval == vprim->u.fval) ? 1 : 0;
  case MV_VALUE_STRING:
    return strcmp(uprim->u.sval, vprim->u.sval) ? 0 : 1;
  }

  assert(0);
  return 0;
}

int mv_value_delete(mv_value_t value)
{
  return 0;
}


int mv_value_print(mv_value_t value)
{
  char *str = _value_to_str(value);
  if (!str)
    return -1;

  fprintf(stdout, "%s\n", str);
  free(str);

  return 0;
}

char *mv_value_to_str(mv_value_t value)
{
  return _value_to_str(value);
}

static void *_null = NULL;
mv_value_t mv_value_null()
{
  if (_null)
    return _VALUE_TAGPTR(_null, MV_VALUE_NULL);

  _null = malloc(sizeof(mv_ptr_t));

  return _VALUE_TAGPTR(_null, MV_VALUE_NULL);
}

int mv_value_is_null(mv_value_t v)
{
  void *ptr = (void *) _VALUE_PTR(v);
  return (ptr == _null) ? 1 : 0;
}

mv_value_t mv_value_int(int v)
{
  _prim_t *prim = malloc(sizeof(_prim_t));
  prim->ptag = MV_VALUE_INT; 
  prim->u.ival = v;

  return _VALUE_TAGPTR(prim, MV_VALUE_INT);
}

int mv_value_int_get(mv_value_t v) 
{
  assert(_VALUE_TAG(v) == MV_VALUE_INT);
  _prim_t *prim = (_prim_t *) _VALUE_PTR(v);
  assert(prim->ptag == MV_VALUE_INT);

  return prim->u.ival;
}

mv_value_t mv_value_float(float v)
{
  _prim_t *prim = malloc(sizeof(_prim_t));
  prim->ptag = MV_VALUE_FLOAT; 
  prim->u.fval = v;

  return _VALUE_TAGPTR(prim, MV_VALUE_FLOAT);
}

float mv_value_float_get(mv_value_t v) 
{
  assert(_VALUE_TAG(v) == MV_VALUE_FLOAT);
  _prim_t *prim = (_prim_t *) _VALUE_PTR(v);
  assert(prim->ptag == MV_VALUE_FLOAT);

  return prim->u.fval;
}

mv_value_t mv_value_string(const char *v)
{
  _prim_t *prim = malloc(sizeof(_prim_t));
  prim->ptag = MV_VALUE_STRING; 
  prim->u.sval = strdup(v);

  return _VALUE_TAGPTR(prim, MV_VALUE_STRING);
}

char *mv_value_string_get(mv_value_t v) 
{
  assert(_VALUE_TAG(v) == MV_VALUE_STRING);
  _prim_t *prim = (_prim_t *) _VALUE_PTR(v);
  assert(prim->ptag == MV_VALUE_STRING);

  return prim->u.sval;
}

mv_value_t mv_value_pair(mv_value_t first, mv_value_t second)
{
  _pair_t *pair = malloc(sizeof(_pair_t));
  pair->first = first;
  pair->second = second;
  return _VALUE_TAGPTR(pair, MV_VALUE_PAIR);
}

mv_value_t mv_value_pair_first(mv_value_t pv)
{
  assert(_VALUE_TAG(pv) == MV_VALUE_PAIR);
  _pair_t *pair = (_pair_t *) _VALUE_PTR(pv);

  return pair->first;
}

mv_value_t mv_value_pair_second(mv_value_t pv)
{
  assert(_VALUE_TAG(pv) == MV_VALUE_PAIR);
  _pair_t *pair = (_pair_t *) _VALUE_PTR(pv);

  return pair->second;
}

mv_value_t mv_value_cons()
{
  _cons_t *cons = malloc(sizeof(_cons_t));
  cons->car = mv_value_null();
  cons->cdr = mv_value_null();

  return _VALUE_TAGPTR(cons, MV_VALUE_CONS);
}

mv_value_t mv_value_cons_car(mv_value_t v)
{
  assert(_VALUE_TAG(v) == MV_VALUE_CONS);
  _cons_t *cons = (_cons_t *) _VALUE_PTR(v);

  return cons->car;
}

mv_value_t mv_value_cons_cdr(mv_value_t v)
{
  assert(_VALUE_TAG(v) == MV_VALUE_CONS);
  _cons_t *cons = (_cons_t *) _VALUE_PTR(v);

  return cons->cdr;
}

mv_value_t mv_value_cons_setcar(mv_value_t cv, mv_value_t v)
{
  assert(_VALUE_TAG(cv) == MV_VALUE_CONS);
  _cons_t *cons = (_cons_t *) _VALUE_PTR(cv);
  
  cons->car = v;

  return cv;
}

mv_value_t mv_value_cons_setcdr(mv_value_t cv, mv_value_t v)
{
  assert(_VALUE_TAG(cv) == MV_VALUE_CONS);
  _cons_t *cons = (_cons_t *) _VALUE_PTR(cv);
  
  cons->cdr = v;

  return cv;
}

mv_value_t mv_value_map()
{
  _map_t *map = malloc(sizeof(_map_t));
  map->bindings = mv_value_null();

  return _VALUE_TAGPTR(map, MV_VALUE_MAP);
}

mv_value_t mv_value_map_lookup(mv_value_t mv, mv_value_t kv)
{
  assert(_VALUE_TAG(mv) == MV_VALUE_MAP);
  _map_t *map = (_map_t *) _VALUE_PTR(mv);

  mv_value_t cons = map->bindings;
  while (!mv_value_is_null(cons)) {
    mv_value_t binding = mv_value_cons_car(cons);
    mv_value_t key = mv_value_pair_first(binding);
    if (mv_value_eq(kv, key)) {
      return mv_value_pair_second(binding);
    }
    cons = mv_value_cons_cdr(cons);
  }

  return mv_value_null();
}

mv_value_t mv_value_map_add(mv_value_t mv, mv_value_t key, mv_value_t val)
{
  assert(_VALUE_TAG(mv) == MV_VALUE_MAP);
  assert(_VALUE_IS_PRIM(_VALUE_TAG(key)));
  _map_t *map = (_map_t *) _VALUE_PTR(mv);

  mv_value_t pair = mv_value_pair(key, val);
  mv_value_t binding = mv_value_cons();
  mv_value_cons_setcar(binding, pair);
  mv_value_cons_setcdr(binding, map->bindings);
  
  map->bindings = binding;

  return mv;
}
