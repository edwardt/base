/**
 * @file rtprop.c
 */
#include <stdio.h>       /* fprintf */
#include <stdlib.h>      /* free, exit */
#include <string.h>      /* strdup */
#include <assert.h>      /* assert */
#include "rtprop.h"
#include "rtobj.h"

typedef struct _rtprop {
  mv_value_t value;       /* value */
} _rtprop_t;


static _rtprop_t *_rtprop_new();
static int _rtprop_delete(_rtprop_t *p);
static int _rtprop_tokenize(char *line, char **name, char **value);
static _rtprop_t *_rtprop_parse(char *line, char **name);

_rtprop_t *_rtprop_new() 
{
  /* later we could change this to property pool-based scheme
     for faster allocation, if needed */
  _rtprop_t *p = malloc(sizeof(_rtprop_t));
  p->value = mv_value_invalid();
  return p;
}

int _rtprop_delete(_rtprop_t *p)
{
  free(p);

  return 0;
}

int _rtprop_tokenize(char *line, char **name, char **value)
{
  char *token;

  /* skip 'property' */
  if ((token = strtok(line, " \t")) == NULL)
    return -1;

  /* get name */
  if ((token = strtok(NULL, " \t")) == NULL)
    return -1;
  *name = token;

  /* get value */
  if ((token = strtok(NULL, " \t")) == NULL)
    return -1;
  *value = token;

  return 0;
}

_rtprop_t *_rtprop_parse(char *line, char **name)
{
  /* Property table contains one property per line:

     P prop0 [value in JSON format]
     P prop1 [value in JSON format]
     ...
  */
  _rtprop_t *prop;
  char *value_s;

  if (_rtprop_tokenize(line, name, &value_s) == -1) {
    fprintf(stderr, "Line not recognized: %s\n", line);
    return NULL;
  }

  if ((prop = _rtprop_new()) == NULL)
    return NULL;

  mv_value_t value_v = mv_value_from_str(value_s);

  /* TODO: increase refcount of value_v */
  prop->value = value_v;

  return prop;
}


/*
 * Functions for the rtprop API.
 */
mvrt_prop_t *mvrt_prop_new(const char *name)
{
  mvrt_obj_t *obj = mvrt_obj_lookup(name, NULL);
  if (obj) {
    fprintf(stderr, "A runtime object already exists with name: %s.\n", name);
    return NULL;
  }

  obj = mvrt_obj_new(name, NULL);
  obj->tag = MVRT_OBJ_PROP;
  obj->data = (void *) _rtprop_new();

  return (mvrt_prop_t *) obj;
}


mvrt_prop_t *mvrt_prop_load_str(char *line)
{
  mvrt_prop_t *rtprop;

  char *name;
  if ((rtprop = _rtprop_parse(line, &name)) == NULL)
    return NULL;

  mvrt_obj_t *obj = mvrt_obj_lookup(name, NULL);
  if (obj) {
    fprintf(stderr, "A runtime object already exists with name: %s.\n", name);
    return NULL;
  }

  obj = mvrt_obj_new(name, NULL);
  obj->tag = MVRT_OBJ_PROP;
  obj->data = (void *) rtprop;

  return (mvrt_prop_t *) obj;
}

char *mvrt_prop_save_str(mvrt_prop_t *p)
{
  static char str[4096];

  mvrt_obj_t *obj = (mvrt_obj_t *) p;
  if (!obj)
    return NULL;

  _rtprop_t *prop = (_rtprop_t *) obj->data;
  char *prop_s = mv_value_to_str(prop->value);
  if (snprintf(str, 4096, "P %s %s", obj->name, prop_s) > 4095) {
    fprintf(stderr, "Buffer overflow.\n");
    return NULL;
  }

  return &str[0];
}

int mvrt_prop_delete(mvrt_prop_t *p)
{
  mvrt_obj_t *obj = (mvrt_obj_t *) p;

  _rtprop_t *prop = obj->data;
  if (_rtprop_delete(prop) == -1)
    return -1;

  return mvrt_obj_delete(obj);
}

int mvrt_prop_delete_by_name(const char *name)
{
  mvrt_obj_t *obj = mvrt_obj_lookup(name, NULL);

  return mvrt_prop_delete((mvrt_prop_t *) obj);
}

mvrt_prop_t *mvrt_prop_lookup(const char *name)
{
  mvrt_obj_t *obj = mvrt_obj_lookup(name, NULL);
  if (!obj)
    return NULL;
  
  assert(obj->tag == MVRT_OBJ_PROP);

  return (mvrt_prop_t *) obj;
}

mv_value_t mvrt_prop_getvalue(mvrt_prop_t *p)
{
  mvrt_obj_t *obj = (mvrt_obj_t *) p;
  if (!obj || obj->tag != MVRT_OBJ_PROP) {
    fprintf(stderr, "Invalid property.\n");
    return mv_value_invalid();
  }

  _rtprop_t *prop = (_rtprop_t *) obj->data;

  /* TODO: for garbage collection, increase the refcount of prop->value */
  return prop->value;
}

mv_value_t mvrt_prop_getvalue_by_name(const char *name)
{
  mvrt_obj_t *obj = mvrt_obj_lookup(name, NULL);

  return mvrt_prop_getvalue((mvrt_prop_t *) obj);
}

int mvrt_prop_setvalue(mvrt_prop_t *p, mv_value_t v)
{
  mvrt_obj_t *obj = (mvrt_obj_t *) p;
  if (!obj || obj->tag != MVRT_OBJ_PROP) {
    fprintf(stderr, "Invalid property.\n");
    return -1;
  }
  
  if (mv_value_is_invalid(v)) {
    fprintf(stderr, "Invalid value.\n");
    return -1;
  }

  _rtprop_t *prop = (_rtprop_t *) obj->data;

  /* TODO: for garbage collection, increase the refcount of v, and
     decrease the refcount of old prop->value */
  prop->value = v;

  return 0;
}

int mvrt_prop_setvalue_by_name(const char *name, mv_value_t v)
{
  mvrt_obj_t *obj = mvrt_obj_lookup(name, NULL);

  return mvrt_prop_setvalue((mvrt_prop_t *) obj, v);
}

