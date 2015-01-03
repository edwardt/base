/**
 * @file rtprop.c
 */
#include <stdio.h>     /* fprintf */
#include <stdlib.h>    /* free, exit */
#include <string.h>    /* strdup */
#include <assert.h>    /* assert */
#include "rtprop.h"


#define MAX_RTPROP_TABLE 4096
typedef struct _rtprop {
  char *dev;
  char *name;
  mv_value_t value;         /* value */

  unsigned tag :      3;    /* tag */
  unsigned free     : 1;    /* free or not */
  unsigned free_idx : 12;   /* index to free list */
  unsigned free_nxt : 12;   /* index of next free entry */
  unsigned pad      : 4;
} _rtprop_t;
static _rtprop_t _rtprop_table[MAX_RTPROP_TABLE];
static _rtprop_t *_free_rtprop;

static int _rtprop_table_init();
static _rtprop_t *_rtprop_get_free();
static int _rtprop_delete(_rtprop_t *prop);
static _rtprop_t *_rtprop_lookup(const char *dev, const char *name);


int _rtprop_table_init(const char *file)
{
  int i;
  _rtprop_t *p;
  for (i = 0; i < MAX_RTPROP_TABLE; i++) {
    p = _rtprop_table + i;
    p->dev = NULL;
    p->name = NULL;
    p->value = mv_value_null();

    p->free = 1;
    p->free_idx = i;
    p->free_nxt = i + 1;
  }
  /* the last element */
  p->free_idx = i;
  p->free_nxt = 0;

  _free_rtprop = _rtprop_table + 1;
  _rtprop_table[0].free = 0;
}

_rtprop_t *_rtprop_get_free()
{
  if (_free_rtprop == _rtprop_table)
    return NULL;

  _rtprop_t *prop = _free_rtprop;
  _free_rtprop = _rtprop_table + prop->free_nxt;
  prop->free = 0;

  return prop;
}

int _rtprop_delete(_rtprop_t *rtprop)
{
  rtprop->free = 1;
  rtprop->free_nxt = _free_rtprop->free_idx;
  _free_rtprop = rtprop;

  return 0;
}

_rtprop_t *_rtprop_lookup(const char *dev, const char *name)
{
  int i;
  _rtprop_t *p;
  for (i = 1; i < MAX_RTPROP_TABLE; i++) {
    p = _rtprop_table + i;
    if (!p->free && !strcmp(p->dev, dev) && !strcmp(p->name, name))
      return p;
  }

  return NULL;
}

/*
 * Functions for the rtprop API.
 */
int mvrt_prop_module_init(const char *file)
{
  _rtprop_table_init(file);
}

mvrt_prop_t mvrt_prop_new(const char *dev, const char *name, int tag)
{
  _rtprop_t *rtprop;

  if ((rtprop = _rtprop_get_free()) == NULL) {
    fprintf(stderr, "Max number of prop reached - %s.\n", name);
    return (mvrt_prop_t) 0;
  }
  if (rtprop->dev) free(rtprop->dev);
  if (rtprop->name) free(rtprop->name);
  rtprop->dev = strdup(dev);
  rtprop->name = strdup(name);
  rtprop->tag = tag;

  switch (tag) {
  case MVRT_PROP_GLOBAL:
  case MVRT_PROP_SYSTEM:
  case MVRT_PROP_LOCAL:
    rtprop->value = mv_value_null();
    return (mvrt_prop_t) rtprop;
  default:
    assert(0 && "mvrt_prop_new: Invalid tag");
  }

  return (mvrt_prop_t) 0;
}

int mvrt_prop_delete(mvrt_prop_t prop)
{
  _rtprop_t *rtprop = (_rtprop_t *) prop;
  return _rtprop_delete(rtprop);
}

mvrt_prop_t mvrt_prop_lookup(const char *dev, const char *name)
{
  _rtprop_t *rtprop;
  if ((rtprop = _rtprop_lookup(dev, name)) == NULL)
    return (mvrt_prop_t) 0;

  return (mvrt_prop_t) rtprop;
}

int mvrt_prop_setvalue(mvrt_prop_t prop, mv_value_t value)
{
  _rtprop_t *rtprop = (_rtprop_t *) prop;
  if (!rtprop)
    return -1;
  
  switch (rtprop->tag) {
  case MVRT_PROP_GLOBAL:
    assert(0 && "Value of a remote property cannot be set directly.");
    return -1;
  case MVRT_PROP_SYSTEM:
  case MVRT_PROP_LOCAL:
    mv_value_delete(rtprop->value);
    rtprop->value = value;
    break;
  default:
    assert(0 && "mvrt_prop_setvalue: Invalid tag");
    break;
  }

  return 0;
}

mv_value_t mvrt_prop_getvalue(mvrt_prop_t prop)
{
  _rtprop_t *rtprop = (_rtprop_t *) prop;
  if (!rtprop)
    return mv_value_null();

  switch (rtprop->tag) {
  case MVRT_PROP_GLOBAL:
    assert(0 && "Remote property cannot be obtained using this function.");
    break;
  case MVRT_PROP_SYSTEM:
  case MVRT_PROP_LOCAL:
    return rtprop->value;
  default:
    assert(0 && "mvrt_prop_setvalue: Invalid tag");
    break;
  }

  return mv_value_null();
}
