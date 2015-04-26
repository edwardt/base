/**
 * @file addr.h
 *
 * @brief Device addresses.
 */
#ifndef MV_ADDR_H
#define MV_ADDR_H

#include <mv/defs.h>     /* mv_ptr_t */

typedef mv_ptr_t mv_addr_t;

typedef enum {
  MV_TRANSPORT_IPv4 = 0,  /* IPv4 */
  MV_TRANSPORT_IPv6,      /* IPv6 */
  MV_TRANSPORT_ZMQ,       /* 0MQ */
  MV_TRANSPORT_ZB,        /* ZigBee */
} mv_transport_t;

#define MV_ADDR_INVALID(adr)  ((adr) == 0)
#define MV_ADDR_TRANSPORT(adr) ((adr) & 0x7)


#ifdef __cplusplus
extern "C" {
#endif

extern mv_addr_t mv_addr(mv_transport_t t, const char *s);

extern int mv_addr_delete(mv_addr_t addr);

extern mv_transport_t mv_addr_transport(mv_addr_t addr);


#ifdef __cplusplus
}
#endif

#endif /* MV_DEVICE_H */
