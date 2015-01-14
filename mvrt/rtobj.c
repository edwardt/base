/**
 * @file rtobj.c
 */
#include <stdio.h>     /* FILE */
#include <stdlib.h>    /* exit */
#include <string.h>    /* memset */
#include <assert.h>    /* assert */
#include "rtprop.h"    /* mvrt_prop_load_str */
#include "rtfunc.h"    /* mvrt_func_load_str */
#include "rtevent.h"   /* mvrt_event_load_str */
#include "rtreactor.h" /* mvrt_reactor_load_str */
#include "rtobj.h"

# define MAX_RTABLE_SIZE  4096
static mvrt_obj_t _rtable[MAX_RTABLE_SIZE];

static mv_uint32_t _objhash(const char *str, const char *dev);

mv_uint32_t _objhash(const char *str, const char *dev)
{

  /* TODO: For now, only str is used for computing hash. */
  size_t hash = 0;
  while (*str)
    hash = hash * 101 + *str++;

  return hash % MAX_RTABLE_SIZE;
}

int _match(mvrt_obj_t *obj, const char *name, const char *dev)
{
  return (obj && !strcmp(obj->name, name) && (!dev || !strcmp(obj->dev, dev)));
}


/*
 * Functions for rtobj API.
 */
static mvrt_obj_t *_rtable_end;
int mvrt_obj_module_init()
{
  size_t mvrt_obj_sz = sizeof(mvrt_obj_t);
  memset(&_rtable[0], 0x0, mvrt_obj_sz * MAX_RTABLE_SIZE);
  
  _rtable_end = _rtable + (MAX_RTABLE_SIZE - 1);
}

mvrt_obj_t *mvrt_obj_new(const char *name, const char *dev)
{
  size_t hash = _objhash(name, dev);
  mvrt_obj_t *p = _rtable + hash;
  mvrt_obj_t *end = (p != _rtable) ? (p - 1) : _rtable_end;

  while (p->used && p != end) {
    p = (p == _rtable_end) ? _rtable : (p + 1);
  }

  if (p->used) {
    fprintf(stderr, "No more free entry.");
    exit(1);
  }

  p->dev = dev ? strdup(name) : NULL;
  p->name = strdup(name);
  p->used = 1;

  fprintf(stdout, "Runtime object created: %s\n", p->name);

  return p;
}

int mvrt_obj_delete(mvrt_obj_t *p)
{
  if (!p)
    return -1;

  p->used = 0;
  if (p->dev) 
    free(p->dev);
  free(p->name);

  return 0;
}

mvrt_obj_t *mvrt_obj_lookup(const char *name, const char *dev)
{
  size_t hash = _objhash(name, dev);
  mvrt_obj_t *p = _rtable + hash;
  mvrt_obj_t *end = (p != _rtable) ? (p - 1) : _rtable_end;

  int matched = 0;
  while (p->used && !(matched = _match(p, name, dev)) && p != end)
    p = (p == _rtable_end) ? _rtable : (p + 1);

  if (!matched)
    return NULL;

  return p;
}

#define MAX_FILE_LINE  4096
int mvrt_obj_loadfile(const char *file)
{
  FILE *fp;
  if ((fp = fopen(file, "r")) == NULL) {
    fprintf(stdout, "mvrt_table_loadfile: Failed to load %s.\n", file);
    return -1;
  }

  char line[MAX_FILE_LINE];
  char *charp;
  mvrt_obj_t *obj;
  while (fgets(line, MAX_FILE_LINE, fp)) {
    if ((charp = strchr(line, '\n')) != NULL)
      *charp = '\0';

    if (strlen(line) == MAX_FILE_LINE) {
      fprintf(stderr, "A line in file, \"%s\", is too long. "
              "Limit line size to < %d.\n", file, MAX_FILE_LINE);
      exit(1);
    }

    /* the fist character must be an alphabet except the body
       of reactor or function */
    if (!line[0] || line[0] == '#' || line[0] == ' ')
      continue;

    mvrt_obj_t *obj = NULL;
    switch (line[0]) {
    case 'p': /* prop name */
      obj = (mvrt_obj_t *) mvrt_prop_load_str(line);
      break;
    case 'e': /* event dev:name */
    case 't': /* timer name t0 t1 */
      obj = (mvrt_obj_t *) mvrt_event_load_str(line);
      break;
    case 'f': /* function name { code } */
    case 'n': /* native name lib.so */
      obj = (mvrt_obj_t *) mvrt_func_load_str(line, fp);
      break;
    case 'r': /* reactor dev:name { code } */
      obj = (mvrt_obj_t *) mvrt_reactor_load_str(line, fp);
      break;
    case 'a':
      obj = (mvrt_obj_t *) mvrt_reactor_assoc_load_str(line);
      break;
    default:
      break;
    }

    if (!obj) {
      fprintf(stderr, "Failed to parse line: %s\n", line);
      continue;
    }
  }

  return 0;
}

int mvrt_obj_savefile(const char *file)
{
  return 0;
}
