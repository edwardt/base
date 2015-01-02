/**
 * @file prop.c
 */
#include <stdio.h>      /* fprintf */
#include <stdlib.h>     /* free, exit */
#include <string.h>     /* strdup */
#include "mv/device.h"  /* mv_device_addr */
#include "mv/value.h"   /* mv_value_t */
#include "mv/prop.h"


#define MAX_PROP_TABLE 4096
typedef struct _prop {
  char *name;               /* prop name = devname + propname */
  mv_value_t value;         /* value */

  unsigned free     : 1;    /* free or not */
  unsigned free_idx : 12;   /* index to free list */
  unsigned free_nxt : 12;   /* index of next free entry */
  unsigned pad      : 7;
} _prop_t;
static _prop_t _prop_table[MAX_PROP_TABLE];
static _prop_t *_free_prop;

static void _prop_table_init();
static _prop_t *_prop_get_free();
static int _prop_delete(_prop_t *prop);
static _prop_t *_prop_lookup(const char *dev, const char *prop);

void _prop_table_init()
{
  int i;
  _prop_t *ptr = _prop_table;

  for (i = 0; i < MAX_PROP_TABLE - 1; i++) {
    ptr->name = NULL;
    ptr->value = mv_value_null();

    ptr->free = 1;
    ptr->free_idx = i;
    ptr->free_nxt = ptr->free_idx + 1;
    ptr++;
  }
  /* the last element */
  ptr->free_idx = i;
  ptr->free_nxt = 0;

  /* The first prop, _prop_list[0], is reserved to represent "no more
     prop". If fnxt == 0, it means no more free prop available. */
  _free_prop = _prop_table + 1;
  _prop_table[0].free = 0;
}

_prop_t *_prop_get_free()
{
  if (_free_prop == _prop_table)
    return NULL;

  _free_prop = _prop_table + _free_prop->free_nxt;
  _free_prop->free = 0;

  return _free_prop;
}

int _prop_delete(_prop_t *prop)
{
  prop->free = 1;
  prop->free_nxt = _free_prop->free_idx;
  if (prop->name)
    free(prop->name);
  _free_prop = prop;

  return 0;
}

_prop_t *_prop_lookup(const char *dev, const char *prop)
{
  int i;
  _prop_t *p;
  
  char *devprop = malloc(sizeof(char) * (strlen(dev) + strlen(prop) + 2));
  sprintf(devprop, "%s:%s", dev, prop);

  for (i = 1; i < MAX_PROP_TABLE - 1; i++) {
    p = _prop_table + i;
    if (p->free)
      continue;
    if (!strcmp(p->name, devprop)) 
      return p;
  }
  
  return 0;
}


/*
 * Functions for the property API.
 */
int mv_prop_service_init()
{
  _prop_table_init();

  return 0;
}

mv_value_t mv_prop_value(const char *dev, const char *prop, int cache)
{
  if (cache) {
    _prop_t *p = _prop_lookup(dev, prop);
    if (p)
      return p->value;
  }

  /* TODO: RPC the device to get the value. Maybe need to split the
     code into two: prop request and return value fetch. */
  

  return mv_value_null();
}
