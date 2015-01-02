/**
 * @file func.h
 *
 * @brief Interface to the remote function call service.
 */
#ifndef MV_FUNC_H
#define MV_FUNC_H

#include <mv/value.h>     /* mv_value_t */

#ifdef __cplusplus
extern "C" {
#endif

/* Initializes the function service. Be sure to call this at time 0. */
extern int mv_func_service_init();

/* Calls a remote function defined in the given device. */
extern mv_value_t mv_func_call(const char *dev, const char *f, mv_value_t args);


#ifdef __cplusplus
}
#endif

#endif /* MV_FUNC_H */
