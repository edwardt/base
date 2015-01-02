/**
 * @file prop.h
 *
 * @brief Interface to the property service, which allows to obtain values
 * of properties of other devices. The name of device and the name of 
 * property uniquely identify the property of a device.
 */
#ifndef MV_PROP_H
#define MV_PROP_H

#include <mv/value.h>     /* mv_value_t */

/* Initializes the event module. Be sure to call this at time 0. */
extern int mv_prop_service_init();

/* Get the value of the given property. Set cache = 1, if cached value of 
 the device is allowed. */
extern mv_value_t mv_prop_value(const char *dev, const char *prop, int cache);

#endif /* MV_PROP_H */
