/**
 * @file rtfunc.c
 */
#include <stdio.h>         /* fprintf */
#include <stdlib.h>        /* free, exit */
#include <string.h>        /* strdup */
#include <assert.h>        /* assert */
#include "rtfunc.h"
#include "rtobj.h"

#define _RTFUNC_MV      0
#define _RTFUNC_NATIVE  1

typedef struct _rtfunc {
  unsigned tag : 1;        /* 0: MV, 1: native */
  unsigned pad : 31;       /* pad */
  union n{
    mvrt_native_t *native; /* tag = 0 */
    mvrt_code_t *code;     /* tag = 1 */
  } u;
} _rtfunc_t;

static _rtfunc_t *rtfunc_new();
static int _rtfunc_delete(_rtfunc_t *f);
static int _rtfunc_token_tag(const char *token);
static int _rtfunc_parse_nargs(int op);
static _rtfunc_t *_rtfunc_parse(char *line, FILE *fp, char **name);


_rtfunc_t *_rtfunc_new()
{
  _rtfunc_t *func = malloc(sizeof(_rtfunc_t));
  if (!func)
    return NULL;
  
  func->u.code = NULL;

  return func;
}

int _rtfunc_delete(_rtfunc_t *func)
{
  if (func->tag == _RTFUNC_MV) {
    if (func->u.code) 
      mvrt_code_delete(func->u.code);
  }
  else if (func->tag == _RTFUNC_NATIVE) {
    if (func->u.native)
      free(func->u.native);
  }

  free(func);

  return 0;
}


int _rtfunc_tokenize(char *line, char **type, char **name, char **lib)
{
  char *token;

  /* get type: "func" or "native" */
  if ((token = strtok(line, " \t")) == NULL)
    return -1;
  *type = token;

  /* get "name" */
  if ((token = strtok(NULL, " \t")) == NULL)
    return -1;
  *name = token;

  /* get "lib" */
  if (!strcmp(*type, "native")) {
    if ((token = strtok(NULL, " \t")) == NULL)
      return -1;
    *lib = token;
  }

  return 0;
}


_rtfunc_t *_rtfunc_parse(char *line, FILE *fp, char **name)
{
  /* func f0 
     {
       getarg 
       pushi 2
       add
     }
 
     native f1 lib.so
  */
  char *token;
  char *type = NULL;
  char *lib = NULL;
  if (_rtfunc_tokenize(line, &type, name, &lib) == -1) {
    fprintf(stderr, "Line not recognized: %s\n", line);
    return NULL;
  }

  _rtfunc_t *func = NULL;
  if (!strcmp(type, "native")) {
    _rtfunc_t *func = _rtfunc_new();
    func->tag = _RTFUNC_NATIVE;
    func->u.native = malloc(sizeof(mvrt_native_t));
    func->u.native->lib = strdup(lib);
    func->u.native->name = strdup(*name);
  }
  else if (!strcmp(type, "func")) {
    mvrt_code_t *code = mvrt_code_load_file(fp);
    if (!code)
      return NULL;
    _rtfunc_t *func = _rtfunc_new();
    func->tag = _RTFUNC_MV;
    func->u.code = code;
  }
  else {
    fprintf(stderr, "Invalid type in line: %s\n", type);
  }

  return func;
}


/*
 * Functions for rtfunc API.
 */
mvrt_func_t *mvrt_func_new(const char *name)
{
  mvrt_obj_t *obj = mvrt_obj_lookup(name, NULL);
  if (obj) {
    fprintf(stderr, "A function already exists with name: %s.\n", name);
    return NULL;
  }

  obj = mvrt_obj_new(name, NULL);
  obj->tag = MVRT_OBJ_FUNC;
  obj->data = (void *) _rtfunc_new();

  return (mvrt_func_t *) obj;
}

mvrt_func_t *mvrt_func_load_str(char *line, FILE *fp)
{
  mvrt_func_t *rtfunc;

  char *name;
  if ((rtfunc = _rtfunc_parse(line, fp, &name)) == NULL)
    return NULL;

  mvrt_obj_t *obj = mvrt_obj_lookup(name, NULL);
  if (obj) {
    fprintf(stderr, "A runtime object already exists with name: %s.\n", name);
    return NULL;
  }

  obj = mvrt_obj_new(name, NULL);
  obj->tag = MVRT_OBJ_FUNC;
  obj->data = (void *) rtfunc;

  return (mvrt_func_t *) obj;
}

int mvrt_func_delete(mvrt_func_t *func)
{
  mvrt_obj_t *obj = (mvrt_obj_t *) func;

  _rtfunc_t *prop = obj->data;
  if (_rtfunc_delete(prop) == -1)
    return -1;

  return mvrt_obj_delete(obj);
}

mvrt_func_t *mvrt_func_lookup(const char *name)
{
  mvrt_obj_t *obj = mvrt_obj_lookup(name, NULL);
  if (!obj)
    return NULL;

  assert(obj->tag == MVRT_OBJ_FUNC);

  return (mvrt_func_t *) obj;
}

int mvrt_func_isnative(mvrt_func_t *func)
{
  mvrt_obj_t *obj = (mvrt_obj_t *) func;
  assert(obj->tag == MVRT_OBJ_FUNC);

  _rtfunc_t *rtfunc = (_rtfunc_t *) obj->data;
  assert(rtfunc);

  return (rtfunc->tag == _RTFUNC_NATIVE) ? 1 : 0;
}

/* Returns the given native function. */
mvrt_native_t *mvrt_func_getnative(mvrt_func_t *func)
{
  mvrt_obj_t *obj = (mvrt_obj_t *) func;
  assert(obj->tag == MVRT_OBJ_FUNC);

  _rtfunc_t *rtfunc = (_rtfunc_t *) obj->data;
  assert(rtfunc);

  return rtfunc->u.native;
}

