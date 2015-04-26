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

#ifdef __cplusplus
extern "C" {
#endif

/* NOTE: For now, actual device (persistent) namespace can be implemented
   in many different ways: files, device name server, or DHT */

/* Initializes the device module. Be sure to call at time 0. */
extern int mv_device_module_init(const char *file);

/* Registers or deregisters a device with the given name. */
extern mv_device_t mv_device_register(const char *name);
extern int mv_device_deregister(mv_device_t dev);

/* Device sign-on and sign-off. To use any device services, one has to
   sign-on first and get a handle to the self device. The mv_device_signon()
   funrtion returns the handle to the self device; on failure, returns 0. 
   The mv_device_signoff() function returns -1 on failure. */
extern mv_device_t mv_device_signon(const char *name, mv_addr_t addr);
extern int mv_device_signoff(mv_device_t dev);

/* Device lookup. */
extern mv_device_t mv_device_self();
extern mv_device_t mv_device_lookup(const char *name);
extern mv_addr_t mv_device_get_transport(mv_device_t dev);


/* Returns the address of the given device */
extern const char *mv_device_addr(const char *name);


#ifdef __cplusplus
}
#endif

#endif /* MV_DEVICE_H */
