/**
 * @file callback.c
 *
 * @author cjeong
 */
#include <stdlib.h>        /* malloc */
#include <strings.h>       /* bzero */
#include <mq/callback.h>

typedef struct _cbstruct {
  mv_cbid_t id;
  mv_cbfunc_t *func;

  unsigned used : 1;
  unsigned tag  : 1;
  unsigned pad  : 30;
} _cbstruct_t;

#define MAX_CALLBACK 4096
typedef struct _cbserver {
  _cbstruct_t callbacks[MAX_CALLBACK];
} _cbserver_t;

static mv_cbserver_t *_cbserver_init();
static mv_cbid_t _callback_get_free(_cbserver_t *cb);
static mv_cbfunc_t *_callback_get(_cbserver_t *cb, mv_cbid_t id);

/*
 * Functions for the Callback API. 
 */
mv_cbid_t _cbid = 0;
mv_cbserver_t *_cbserver_init()
{
  int i;
  _cbserver_t *cb = malloc(sizeof(_cbserver_t));
  _cbid = 1;

  bzero(&cb->callbacks[0], sizeof(_cbstruct_t) * MAX_CALLBACK);

  return (mv_cbserver_t *) cb;
}

mv_cbid_t _callback_get_free(_cbserver_t *cb)
{
  int i;
  for (i = 0; i < _cbid; i++) {
    if (!cb->callbacks[i].used)
      return i;
  }

  return MAX_CALLBACK;
}


mv_cbid_t mv_callback_add(mv_cbserver_t *mvcb, mv_cbfunc_t *fun, mv_cbtag_t t)
{
  _cbserver_t *cb = (_cbserver_t *) mvcb;
  mv_cbid_t id = _callback_get_free(cb);
  if (id == MAX_CALLBACK) {
    return 0;
  }
  cb->callbacks[id].id = id;
  cb->callbacks[id].func = fun;
  cb->callbacks[id].tag = t;

  return id;
}

mv_cbfunc_t *mv_callback_get(mv_cbserver_t *mvcb, mv_cbid_t id)
{
  _cbserver_t *cb = (_cbserver_t *) mvcb;
  int i;
  for (i = 0; i < _cbid; i++) {
    if (!cb->callbacks[i].used) 
      continue;
    
    if (cb->callbacks[i].tag == MV_CB_ONETIME) {
      cb->callbacks[i].used = 0;
      return cb->callbacks[i].func;
    }
  }

  return (mv_cbfunc_t *) 0;
}
