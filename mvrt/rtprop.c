/**
 * @file rtprop.c
 */
#include <stdio.h>       /* fprintf */
#include <stdlib.h>      /* free, exit */
#include <string.h>      /* strdup */
#include <assert.h>      /* assert */
#include <mv/device.h>   /* mv_device_self */
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
static int _rtprop_loadfile(const char *file, mvrt_proptag_t tag);
static int _rtprop_savefile(const char *file, mvrt_proptag_t tag);
static int _rtprop_tokenize(char *line, char **name, char **value);


int _rtprop_table_init()
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

int _rtprop_tokenize(char *line, char **name, char **value)
{
  char *token;
  if ((token = strtok(line, " \t")) == NULL)
    return -1;
  *name = token;
  if ((token = strtok(NULL, " \t")) == NULL)
    return -1;
  *value = token;
  return 0;
}

int _rtprop_loadfile(const char *file, mvrt_proptag_t tag)
{
  FILE *fp;
  if ((fp = fopen(file, "r")) == NULL) {
    fprintf(stderr, "Failed to open %s\n", file);
    return -1;
  }

  /* Property table contains one property per line:

     prop0 [value in JSON format]
     prop1 [value in JSON format]
     ...
  */
  const char *self = mv_device_self();

  char line[1024];
  char *name;
  char *value_s;
  mvrt_prop_t prop;
  while (fgets(line, 1024, fp)) {
    char *charp = strstr(line, "\n");
    if (charp)
      *charp = '\0';

    if (line[0] == '#')
      continue;

    if (_rtprop_tokenize(line, &name, &value_s) == -1) {
      fprintf(stderr, "Line not recognized: %s\n", line);
      continue;
    }

    prop = mvrt_prop_new(self, name, tag);
    if (!prop)
      continue;

    mv_value_t value_v = mv_value_from_str(value_s);
    mvrt_prop_setvalue(prop, value_v);
  }

  fprintf(stdout, "Property file loaded: %s...\n", file);
}

int _rtprop_savefile(const char *file, mvrt_proptag_t tag)
{
  return 0;
}

/*
 * Functions for the rtprop API.
 */
int mvrt_prop_module_init()
{
  _rtprop_table_init();
  fprintf(stdout, "Property module initialized...\n");

  return 0;
}

mvrt_prop_t mvrt_prop_new(const char *dev, const char *name, int tag)
{
  if (mvrt_prop_lookup(dev, name) != (mvrt_prop_t) 0) {
    fprintf(stderr, "Duplicate property: %s:%s\n", dev, name);
    return (mvrt_prop_t) 0;
  }

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

int mvrt_prop_loadfile(const char *file, mvrt_proptag_t tag)
{
  return _rtprop_loadfile(file, tag);
}

int mvrt_prop_savefile(const char *file, mvrt_proptag_t tag)
{
  return _rtprop_savefile(file, tag);
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
