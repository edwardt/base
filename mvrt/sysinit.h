/**
 * @file sysinit.h
 *
 * @brief Interface to system events and reactors.
 */
#ifndef MVRT_SYSINIT_H
#define MVRT_SYSINIT_H

#include "rtevent.h"         /* mvrt_event_t */
#include "rtfunc.h"          /* mvrt_func_t */
#include "rtprop.h"          /* mvrt_prop_t */
#include "reactor.h"         /* mvrt_reactor_t */

typedef enum {
  MVRT_SYSEV_PROP_GET,
  MVRT_SYSEV_PROP_SET,
  MVRT_SYSEV_FUNC_CALL,
  MVRT_SYSEV_FUNC_CALL_RET,
  MVRT_SYSEV_FUNC_RETURN,
  MVRT_SYSEV_NTAGS
} mvrt_sysevent_t;


typedef enum {
  MVRT_SYSRE_PROP_GET,
  MVRT_SYSRE_PROP_SET,
  MVRT_SYSRE_FUNC_CALL,
  MVRT_SYSRE_FUNC_CALL_RET,
  MVRT_SYSRE_FUNC_RETURN,
  MVRT_SYSRE_NTAGS
} mvrt_sysreactor_t;


typedef enum {
  MVRT_SYSFN_NTAGS
} mvrt_sysfunc_t;

typedef enum {
  MVRT_SYSPR_NTAGS
} mvrt_sysprop_t;


extern void mvrt_system_event_init();
extern mvrt_event_t mvrt_system_event_get(mvrt_sysevent_t id);

extern void mvrt_system_func_init();
extern mvrt_func_t mvrt_system_func_get(mvrt_sysfunc_t id);

extern void mvrt_system_prop_init();
extern mvrt_func_t mvrt_system_prop_get(mvrt_sysprop_t id);

extern void mvrt_system_reactor_init();
extern mvrt_reactor_t *mvrt_system_reactor_get(mvrt_sysreactor_t id);


#endif /* MVRT_SYSINIT_H */
