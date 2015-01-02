/**
 * @file mdecoder.c
 *
 * @author cjeong
 */
#include <stdio.h>       /* printf */
#include <stdlib.h>      /* malloc, free */
#include <pthread.h>     /* pthread_create */
#include <time.h>        /* nanosleep */
#include <assert.h>      /* aasert */
#include <mv/device.h>   /* mv_device_self */
#include "rtprop.h"      /* mvrt_prop_t */
#include "rtfunc.h"      /* mvrt_func_t */
#include "rtvalue.h"     /* mvrt_value_t */
#include "message.h"     /* mvrt_message_t */
#include "mdecoder.h"


typedef struct {
  mv_mqueue_t *mq;
  mvrt_evqueue_t *evq;
  pthread_t thr;
} _mdecoder_t;

static void *_mdecoder_thread(void *arg);
static mvrt_value_t _mdecoder_decode(mvrt_message_t *mvmsg);


void *_mdecoder_thread(void *arg)
{
  _mdecoder_t *mf = (_mdecoder_t *) arg;    /* decoder */
  mv_mqueue_t *mq = mf->mq;                 /* message queue */
  mvrt_evqueue_t *evq = mf->evq;            /* event queue */

  char *str ;                               /* message string from mq */
  mvrt_message_t *mvmsg;                    /* mvrt_message */
  mvrt_event_t *evt;                        /* event from decoder */
  struct timespec ts;                       /* time for nanosleep */

  mvrt_value_t null_v = mvrt_value_null();  /* null value */
  mvrt_value_t ev;                          /* event value */

  ts.tv_sec = 0;
  ts.tv_nsec = 5000000;

  
  while (1) {
    char *str = mv_mqueue_get(mq);
    if (!str) {
      // sleep for 5ms
      nanosleep(&ts, NULL);
      continue;
    }

    fprintf(stdout, "Message fetched from queue: %s\n", str);
    mvmsg = mvrt_message_parse(str);
    if (!mvmsg)
      continue;

    fprintf(stdout, "Message decoded from queue: %s => %d\n", str, mvmsg->tag);
    free(str);

    ev = _mdecoder_decode(mvmsg);
    if (ev == null_v)
      continue;

    while (mvrt_evqueue_full(evq)) {
      nanosleep(&ts, NULL);
    }
    mvrt_evqueue_put(evq, ev);
  }

  pthread_exit(NULL);
}

enum {
  _V_STRING_NAME   = 0,
  _V_STRING_ARG    = 1,
  _V_STRING_FUNARG = 2,
  _V_NTAGS
};
static mvrt_value_t _values[_V_NTAGS];
static int _mdecoder_init_done = 0;

void _mdecoder_init()
{
  if (_mdecoder_init_done)
    return;

  _values[_V_STRING_NAME] = mvrt_value_string("name");
  _values[_V_STRING_ARG] = mvrt_value_string("arg");
  _values[_V_STRING_FUNARG] = mvrt_value_string("funarg");
  _mdecoder_init_done = 1;
}

mvrt_value_t _mdecoder_decode(mvrt_message_t *mvmsg)
{
  mvrt_value_t arg;       /* arg of the message */
  mvrt_value_t name;      /* name */

  mvrt_event_t event;     /* event type */
  mvrt_prop_t prop;       /* property */
  mvrt_func_t func;       /* function */

  mvrt_value_t event_v;   /* event instance */
  mvrt_value_t farg_v;    /* function arg */
  mvrt_value_t func_v;    /* function value */
  mvrt_value_t pair_v;    /* pair value */
  mvrt_value_t null_v;    /* null value */

  char *namestr;        /* name string */

  switch (mvmsg->tag) {
  case MVRT_MESSAGE_EVENT_OCCUR:
    /* 
       { 
         tag: "EVENT_OCCUR",
         arg: { "name": "adjust_volume", "volume": 12 } 
       }
    */
    arg = mvmsg->arg;
    name = mvrt_value_map_lookup(arg, _values[_V_STRING_NAME]);
    namestr = mvrt_value_string_get(name);
    event = mvrt_event_lookup(mvmsg->srcdev, namestr);
    if (!event) {
      fprintf(stdout, "ERROR: No such event -- %s.\n", namestr);
      break;
    }
    if (mvrt_value_event(event, arg) == null_v) {
      fprintf(stdout, "Failed to create an event instance.\n", namestr);
      return null_v;
    }
    break;
  case MVRT_MESSAGE_PROP_ADD:
    /* 
       { 
         tag: "PROP_ADD",
         arg: { "name": "current_volume" } 
       }
    */
    arg = mvmsg->arg;
    name = mvrt_value_map_lookup(arg, _values[_V_STRING_NAME]);
    namestr = mvrt_value_string_get(name);
    prop = mvrt_prop_new(mv_device_self(), namestr, MVRT_PROP_LOCAL);
    if (!prop) {
      fprintf(stdout, "ERROR: Failed to create property -- %s.\n", namestr);
      return null_v;
    }
    break;
  case MVRT_MESSAGE_PROP_SET:
    {
    }
    break;
  case MVRT_MESSAGE_PROP_GET:
    {
    }
    break;
  case MVRT_MESSAGE_FUNC_CALL:
    /* 
       { 
         tag: "FUNC_CALL",
         arg: { "name": "adjust_volume", "arg": 14 } 
       }
    */
    arg = mvmsg->arg;
    name = mvrt_value_map_lookup(arg, _values[_V_STRING_NAME]);
    farg_v = mvrt_value_map_lookup(arg, _values[_V_STRING_FUNARG]);
    namestr = mvrt_value_string_get(name);
    func = mvrt_func_lookup(mv_device_self(), namestr);
    fprintf(stdout, "fcall: %s\n", namestr);
    if (!func) {
      fprintf(stdout, "ERROR: No such function -- %s.\n", namestr);
      return null_v;
    }
    func_v = mvrt_value_func(func);
    //printf("FUNC: "); mvrt_value_print(func_v);
    pair_v = mvrt_value_pair(func_v, farg_v);

    switch (mvrt_func_tag(func)) {
    case MVRT_FUNC_NATIVE:
      event = mvrt_event_lookup(mv_device_self(), ":native_call");
      event_v = mvrt_value_event(event, pair_v);
      //mvrt_value_print(pair_v);
      //mvrt_value_print(event_v);
      break;
    case MVRT_FUNC_GLOBAL:
      event = mvrt_event_lookup(mv_device_self(), ":func_call");
      event_v = mvrt_value_event(event, pair_v);
      break;
    case MVRT_FUNC_LOCAL:
    default:
      fprintf(stdout, "ERROR: No such function -- %s.\n", namestr);
      return null_v;
    }
    return event_v;
  case MVRT_MESSAGE_INVALID:
  default:
    assert(0 && "Invalid message type in _mdecoder_decoder");
    break;
  }

  return null_v;
}

/*
 * Functions for the decoder API.
 */
mvrt_mdecoder_t *mvrt_mdecoder(mv_mqueue_t *mq, mvrt_evqueue_t *evq)
{
  _mdecoder_init();

  _mdecoder_t *mf = (_mdecoder_t *) malloc(sizeof(_mdecoder_t));
  if (!mf)
    return NULL;

  mf->mq = mq;
  mf->evq = evq;

  return mf;
}

int mvrt_mdecoder_run(mvrt_mdecoder_t *f)
{
  _mdecoder_t *mf = (_mdecoder_t *) f;

  if (pthread_create(&mf->thr, NULL, _mdecoder_thread, mf) != 0) {
    perror("pthread_create@mvrt_message_mdecoder_run");
    return -1;
  }
  return 0;
}


