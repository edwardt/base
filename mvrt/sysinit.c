/**
 * @file sysinit.c
 */
#include <stdio.h>       /* fprintf */
#include <assert.h>      /* assert */
#include <mv/device.h>   /* mv_device_self */
#include "sysinit.h"


static mvrt_event_t _sysevents[MVRT_SYSEV_NTAGS];
static const char *_sysevent_str[] = {
  "_E_prop_get",
  "_E_prop_set",
  "_E_func_call",
  "_E_func_call_ret",
  "_E_func_return",
  ""
};

static mvrt_func_t _sysfuncs[MVRT_SYSFN_NTAGS];
static const char *_sysfunc_str[] = {
  ""
};

static mvrt_prop_t _sysprops[MVRT_SYSPR_NTAGS];
static const char *_sysprop_str[] = {
  ""
};

static mvrt_reactor_t *_sysreactors[MVRT_SYSEV_NTAGS];
static const char *_sysreactor_str[] = {
  "_R_prop_get",
  "_R_prop_set",
  "_R_func_call",
  "_R_func_call_ret",
  "_R_func_return",
  ""
};

void mvrt_system_event_init()
{
  int i;
  const char *self = mv_device_self();
  for (i = 0; i < MVRT_SYSEV_NTAGS; i++)
    _sysevents[i] = mvrt_event_new(self, _sysevent_str[i], MVRT_EVENT_SYSTEM);
}

mvrt_func_t mvrt_system_event_get(mvrt_sysevent_t id)
{
  return _sysevents[id];
}

void mvrt_system_func_init()
{
  int i;
  /* 
     const char *self = mv_device_self();
     for (i = 0; i < MVRT_SYSFN_NTAGS; i++)
     _sysfuncs[i] = mvrt_func_new(self, _sysevent_str[i], MVRT_FUNC_SYSTEM);
  */
}

mvrt_func_t mvrt_system_func_get(mvrt_sysfunc_t id)
{
  return _sysfuncs[id];
}

void mvrt_system_prop_init()
{
  /* 
     int i;
     const char *self = mv_device_self();
     for (i = 0; i < MVRT_SYSEV_NTAGS; i++)
     _sysprops[i] = mvrt_prop_new(self, _sysprop_str[i], MVRT_PROP_SYSTEM);
  */
}

mvrt_prop_t mvrt_system_prop_get(mvrt_sysprop_t id)
{
  return _sysprops[id];
}


void mvrt_system_reactor_init()
{
  int i;
  const char *name;
  mvrt_event_t sysev;
  mvrt_reactor_t *sysre;
  for (i = 0; i < MVRT_SYSRE_NTAGS; i++) {
    name = _sysreactor_str[i]; 
    _sysreactors[i] = mvrt_reactor_lookup(name);
    if (!_sysreactors[i]) {
      fprintf(stderr, "System reactor, %s, not defined.\n", name);
      continue;
    }

    sysev = mvrt_system_event_get(i);
    assert(sysev && "System event not defined.");
    mvrt_add_reactor_to_event(sysev, _sysreactors[i]);
  }
}

mvrt_reactor_t *mvrt_system_reactor_get(mvrt_sysreactor_t id)
{
  return _sysreactors[id];
}

