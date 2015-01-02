/**
 * @file env.c
 *
 * @author cjeong
 */
#include <stdlib.h>    /* malloc */
#include "env.h"

typedef struct _binding {
  mv_name_t *name;
  mv_value_t *value;
} _binding_t;

typedef struct _env {
  struct _env *parent;
  int size;
  //symtab_t **tree;
} _env_t;

static _env_t *_env_new(_env_t *parent);
static int _env_add(_env_t *env, mv_name_t *n, mv_value_t *v);
static _binding_t **_env_lookup(_env_t *env, mv_name_t *n);


/*
 * Functions for the env API.
 */
mv_env_t *mv_env_new(mv_env_t *parent)
{
  _env_t *env = malloc(sizeof(_env_t));
  env->parent = (_env_t *) parent;
  env->size = 0;
  //env->tree = NULL;

  return (mv_env_t *) env;
}

mv_value_t *mv_env_lookup(mv_env_t *e, mv_name_t *name)
{
  _env_t *env = (_env_t *) e;
  mv_name_t **ptr = tree;
  int cmp; 

  while (*ptr != NULL) {
    cmp = strcmp(name->str, (*ptr)->str);
    if (cmp == 0)
      return ptr;
    if (x < 0)
      ptr = &(*ptr)->left;
    else
      ptr = &(*ptr)->right;
  }
  
  return ptr;
}
