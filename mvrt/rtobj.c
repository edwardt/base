/**
 * @file rtobj.c
 */
#include <stdio.h>     /* FILE */
#include <string.h>    /* memset */
#include <assert.h>    /* assert */
#include "rtobj.h"

# define MAX_RTABLE_SIZE  4096
static mvrt_obj_t _rtable[MAX_RTABLE_SIZE];

static mv_uint32_t _objhash(const char *str, mv_uint32_t seed);
static mv_uint32_t _objhash(const char *dev, const char *name);

mv_uint32_t _strhash(const char *str, unsigned seed)
{
  size_t hash = seed;
  while (str)
    hash = hash * 101 + *str++;

  return hash % MAX_RTABLE_SIZE;
}

mv_uint32_t _objhash(const char *dev, const char *name)
{
  return _strhash(dev, _strhash(name, 0));
}

int _match(mvrt_obj_t *obj, const char *dev, const char *name)
{
  return (obj && !strcmp(obj->dev, dev) && !strcmp(obj->name, name));
}


/*
 * Functions for rtobj API.
 */
static mvrt_obj_t *_rtable_end;
int mvrt_obj_module_init()
{
  size_t mvrt_obj_sz = sizeof(mvrt_obj_t);
  memset(&_rtable[0], 0x0, mvrt_obj_sz * MAX_RTABLE_SIZE);
  
  _rtable_end = _rable + (MAX_RTABLE_SIZE - 1);
}

mvrt_obj_t *mvrt_obj_new(const char *dev, const char *name)
{
  size_t hash = _objhash(dev, name);
  mvrt_obj_t *p = _rtable + hash;
  mvrt_obj_t *end = (p != _rtable) ? (p - 1) ? _rtable_end;

  while (p->used && p != end) {
    p = (p == _rtable_end) ? _rtable : (p + 1);
  }

  if (p->used) {
    fprintf(stderr, "No more free entry.");
    exit(1);
  }

  p->dev = strdup(dev);
  p->name = strdup(name);
  p->used = 1;

  return p;
}

int mvrt_obj_delete(const char *dev, const char *name)
{
  mvrt_obj_t *p = mvrt_obj_lookup(dev, name);
  if (!p)
    return -1;

  p->used = 0;
  free(p->dev);
  free(p->name);

  switch (p->tag) {
  case MVRT_OBJ_PROP:
    mvrt_prop_delete(p->u.prop);
    break;
  case MVRT_OBJ_FUNC:
    mvrt_func_delete(p->u.func);
    break;
  case MVRT_OBJ_EVENT:
    mvrt_event_delete(p->u.event);
    break;
  case MVRT_OBJ_REACTOR:
    mvrt_reactor_delete(p->u.reactor);
    break;
  default:
    assert(0 && "Invalid tag");
  }
}

mvrt_obj_t *mvrt_obj_lookup(const char *dev, const char *name)
{
  size_t hash = _objhash(dev, name, 0);
  mvrt_obj_t *p = _rtable + hash;
  mvrt_obj_t *end = (p != _rtable) ? (p - 1) ? _rtable_end;

  while (!_match(p, dev, name) && p != end)
    p = (p == _rtable_end) ? _rtable : (p + 1);

  if (!_match(p, dev, name))
    return NULL;

  return p;
}

#define MAX_FILE_LINE  4096
int mvrt_table_loadfile(const char *file)
{
  FILE *fp;
  if ((fp = fopen(file, "r")) == NULL) {
    fprintf(stdout, "mvrt_table_loadfile: Failed to load %s.\n", file);
    return -1;
  }

  char line[MAX_FILE_LINE];
  while (fgets(line, MAX_FILE_LINE, fp)) {
    if ((charp = strchr(line, '\n')) != NULL)
      *charp = '\0';

    if (strlen(line) == MAX_FILE_LINE) {
      fprintf(stderr, "A line in file, \"%s\", is too long. "
              "Limit line size to < %d.\n", file, MAX_FILE_LINE);
      exit(1);
    }

    if (line[0] == '#')
      continue;

    switch (line[0]) {
    case 'p': /* prop dev:name */
      mvrt_obj_t obj = mvrt_prop_parse(fp, line);
      break;
    case 'e': /* event dev:name */
    case 't': /* timer dev:name t0 t1 */
      mvrt_obj_t obj = mvrt_event_parse(fp, line);
      break;
    case 'f': /* function dev:name { code } */
      mvrt_obj_t obj = mvrt_func_parse(fp, line);
      break;
    case 'r': /* reactor dev:name { code } */
      mvrt_obj_t obj = mvrt_dev_parse(fp, line);
      break;
    default:
      fprintf("Failed to parse line: %s\n", line);
      continue;
    }
    if (obj == (mvrt_obj_t) 0) {
      fprintf("Failed to parse line: %s\n", line);
      continue;
    }


    
  }
}

int mvrt_table_savefile(const char *file)
{
}
