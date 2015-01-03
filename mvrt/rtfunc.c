/**
 * @file rtfunc.c
 */
#include <stdio.h>       /* fprintf */
#include <stdlib.h>      /* free, exit */
#include <string.h>      /* strdup */
#include <assert.h>      /* assert */
#include <mv/device.h>   /* mv_device_self */
#include "rtfunc.h"


#define MAX_RTFUNC_TABLE 4096
typedef struct _rtfunc {
  char *dev;
  char *name;
  union {
    mvrt_native_t *native;  /* NATIVE */
  } u;
  unsigned tag :      3;    /* tag */
  unsigned free     : 1;    /* free or not */
  unsigned free_idx : 12;   /* index to free list */
  unsigned free_nxt : 12;   /* index of next free entry */
  unsigned pad      : 4;
} _rtfunc_t;
static _rtfunc_t _rtfunc_table[MAX_RTFUNC_TABLE];
static _rtfunc_t *_free_rtfunc;

static int _rtfunc_table_init();
static _rtfunc_t *_rtfunc_get_free();
static int _rtfunc_delete(_rtfunc_t *func);
static _rtfunc_t *_rtfunc_lookup(const char *dev, const char *name);
static int _rtfunc_loadfile(const char *file, mvrt_functag_t tag);
static int _rtfunc_savefile(const char *file, mvrt_functag_t tag);
static int _rtfunc_tokenize(char *line, char **tag, char **lib, char **fun);

int _rtfunc_table_init()
{
  int i;
  _rtfunc_t *f;
  for (i = 0; i < MAX_RTFUNC_TABLE; i++) {
    f = _rtfunc_table + i;
    f->dev = NULL;
    f->name = NULL;

    f->free = 1;
    f->free_idx = i;
    f->free_nxt = i + 1;
  }
  /* the last element */
  f->free_idx = i;
  f->free_nxt = 0;

  _free_rtfunc = _rtfunc_table + 1;
  _rtfunc_table[0].free = 0;
}

_rtfunc_t *_rtfunc_get_free()
{
  if (_free_rtfunc == _rtfunc_table)
    return NULL;

  _rtfunc_t *func = _free_rtfunc;
  _free_rtfunc = _rtfunc_table + func->free_nxt;
  func->free = 0;

  return func;
}

int _rtfunc_delete(_rtfunc_t *rtfunc)
{
  rtfunc->free = 1;
  rtfunc->free_nxt = _free_rtfunc->free_idx;
  _free_rtfunc = rtfunc;

  if (rtfunc->tag == MVRT_FUNC_NATIVE) {
    free(rtfunc->u.native->lib);
    free(rtfunc->u.native->name);
    free(rtfunc->u.native);
  }

  return 0;
}

_rtfunc_t *_rtfunc_lookup(const char *dev, const char *name)
{
  int i;
  _rtfunc_t *p;
  for (i = 1; i < MAX_RTFUNC_TABLE; i++) {
    p = _rtfunc_table + i;
    if (!p->free && !strcmp(p->dev, dev) && !strcmp(p->name, name))
      return p;
  }

  return NULL;
}

int _rtfunc_tokenize(char *line, char **type, char **lib, char **fun)
{
  char *token;
  if ((token = strtok(line, " \t")) == NULL)
    return -1;
  *type = token;
  if ((token = strtok(NULL, " \t")) == NULL)
    return -1;
  *lib = token;
  if ((token = strtok(NULL, " \t")) == NULL)
    return -1;
  *fun = token;
  return 0;
}

int _rtfunc_loadfile(const char *file, mvrt_functag_t tag)
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
  char *type;
  char *lib;
  char *name;
  _rtfunc_t *rtfunc;
  const char *dev = mv_device_self();

  while (fgets(line, 1024, fp)) {
    char *charp = strstr(line, "\n");
    if (charp)
      *charp = '\0';

    if (_rtfunc_tokenize(line, &type, &lib, &name) == -1) {
      fprintf(stderr, "Line not recognized: %s\n", line);
      continue;
    }
    /* Currently, only native functions supported */
    if (strcmp(type, "native"))
      continue;

    rtfunc = (_rtfunc_t *) mvrt_func_new(dev, name, MVRT_FUNC_NATIVE);
    rtfunc->u.native->name = strdup(name);
    rtfunc->u.native->lib = strdup(lib);
  }
}

int _rtfunc_savefile(const char *file, mvrt_functag_t tag)
{
  return 0;
}



/*
 * Functions for the rtfunc API.
 */
int mvrt_func_module_init(const char *file)
{
  _rtfunc_table_init();
  fprintf(stdout, "Function module initialized...\n");
  
  return 0;
}

mvrt_func_t mvrt_func_new(const char *dev, const char *name, int tag)
{
  _rtfunc_t *rtfunc;

  if ((rtfunc = _rtfunc_get_free()) == NULL) {
    fprintf(stderr, "MAX table reached - %s.\n", name);
    return (mvrt_func_t) 0;
  }

  if (rtfunc->dev) free(rtfunc->dev);
  if (rtfunc->name) free(rtfunc->name);
  rtfunc->dev = strdup(dev);
  rtfunc->name = strdup(name);
  rtfunc->tag = tag;

  switch (tag) {
  case MVRT_FUNC_GLOBAL:
  case MVRT_FUNC_SYSTEM:
  case MVRT_FUNC_LOCAL:
    return (mvrt_func_t) rtfunc;
  case MVRT_FUNC_NATIVE:
    rtfunc->u.native = malloc(sizeof(mvrt_native_t));
    rtfunc->u.native->lib = NULL;
    rtfunc->u.native->name = NULL;
    rtfunc->u.native->func1 = (mvrt_native_func1_t) 0;
    rtfunc->u.native->func2 = (mvrt_native_func2_t) 0;
    return (mvrt_func_t) rtfunc;
  default:
    assert(0 && "mvrt_func_new: Invalid tag");
  }

  return (mvrt_func_t) 0;
}

int mvrt_func_delete(mvrt_func_t func)
{
  _rtfunc_t *rtfunc = (_rtfunc_t *) func;
  return _rtfunc_delete(rtfunc);
}

mvrt_func_t mvrt_func_lookup(const char *dev, const char *name)
{
  _rtfunc_t *rtfunc;
  if ((rtfunc = _rtfunc_lookup(dev, name)) == NULL)
    return (mvrt_func_t) 0;

  return (mvrt_func_t) rtfunc;
}

int mvrt_func_loadfile(const char *file, mvrt_functag_t tag)
{
  return _rtfunc_loadfile(file, tag);
}

int mvrt_func_savefile(const char *file, mvrt_functag_t tag)
{
  return _rtfunc_savefile(file, tag);
}

const char *mvrt_func_dev(mvrt_func_t func)
{
  _rtfunc_t *rtfunc = (_rtfunc_t *) func;
  return rtfunc->dev;
}

const char *mvrt_func_name(mvrt_func_t func)
{
  _rtfunc_t *rtfunc = (_rtfunc_t *) func;
  return rtfunc->name;
}

mvrt_functag_t mvrt_func_tag(mvrt_func_t func)
{
  _rtfunc_t *rtfunc = (_rtfunc_t *) func;
  return rtfunc->tag;
}

mvrt_native_t *mvrt_func_native(mvrt_func_t func)
{
  _rtfunc_t *rtfunc = (_rtfunc_t *) func;
  assert((rtfunc->tag == MVRT_FUNC_NATIVE) && "Invalid tag.");
  return rtfunc->u.native;
}
