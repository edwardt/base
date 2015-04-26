/**
 * @file device.h
 *
 * @brief Interface to the device layer which matains namespaces of devices. 
 */
#ifndef MV_DEVICE_H
#define MV_DEVICE_H

#include <mv/defs.h>     /* mv_ptr_t */
#include <mv/addr.h>     /* mv_addr_t */

/* Opaque pointer to a device. */
typedef mv_ptr_t mv_device_t;

#define MV_DEVICE_INVALID(dev)  ((dev) == 0)


#ifdef __cplusplus
extern "C" {
#endif

/* NOTE: For now, actual device (persistent) namespace can be implemented
   in many different ways: files, device name server, or DHT */

/* Call this at time 0. */
extern int mv_device_module_init(const char *file);

extern mv_device_t mv_device_register(const char *name);
extern int mv_device_deregister(mv_device_t dev);

extern mv_device_t mv_device_signon(const char *name, mv_addr_t addr);
extern int mv_device_signoff(mv_device_t dev);
extern mv_device_t mv_device_self();

extern mv_device_t mv_device_lookup(const char *name);
extern const char *mv_device_name(mv_device_t dev);
extern mv_addr_t mv_device_addr(mv_device_t dev);


#ifdef __cplusplus
}
#endif

#endif /* MV_DEVICE_H */
