/**
 * @file device.h
 *
 * @brief Interface to the device service, which allows to lookup devices 
 * based on a globally-unique name. Preferrably, we can implement this layer
 * in a distributed way.
 */
#ifndef MV_DEVICE_H
#define MV_DEVICE_H


#include <mv/defs.h>     /* mv_ptr_t */

#ifdef __cplusplus
extern "C" {
#endif

/* NOTE: For now, actual device (persistent) database exists in a central
   server. So, this device API is used to access the database server to
   lookup device information -- in particular, what is the current address
   of the given device, if signed on. */

/* Initializes the device layer. Be sure to call at time 0. */
extern int mv_device_service_init(const char *dev, const char *file);

/* Registers or deregisters a device with the given name. */
extern int mv_device_register(const char *name);
extern int mv_device_deregister(const char *name);

/* Device signs on and off. Each device runtime should call signon function, 
   when first connects to the device service. */
extern int mv_device_signon(const char *name, const char *addr);
extern int mv_device_signoff(const char *name);

extern const char *mv_device_self();

/* Returns the address of the given device */
extern const char *mv_device_addr(const char *name);

//extern mv_addr_t mv_device_addr(mv_transport_t t);

#ifdef __cplusplus
}
#endif

#endif /* MV_DEVICE_H */
