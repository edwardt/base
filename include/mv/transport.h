/**
 * @file transport.h
 *
 * @brief Network transport interface.
 */
#ifndef MV_TRANSPORT_H
#define MV_TRANSPORT_H

#include <mv/defs.h>      /* mv_ptr_t */

typedef mv_ptr_t mv_transport_t;

typedef enum {
  MV_TRANSPORT_TCP = 0,   /* TCP: e.g. tcp://192.168.8.102:1234 */
  MV_TRANSPORT_UDP,       /* UDP: e.g. udp://192.168.8.102:1234 */
  MV_TRANSPORT_ZMQ,       /* 0MQ: e.g. zmq://192.168.8.102 */
  MV_TRANSPORT_MQTT,      /* MQTT */
  MV_TRANSPORT_ZB,        /* ZigBee */
  MV_TRANSPORT_NTAGS
} mv_ttag_t;

#define MV_TRANSPORT_INVALID(transport)  ((transport) == 0)

#ifdef __cplusplus
extern "C" {
#endif

extern mv_transport_t mv_transport_open(const char *s);
extern int mv_transport_close(mv_transport_t t);

typedef int (*transport_callback_t)(char *s);
typedef int (*transport_callback_t)(char *s);

int mv_transport_send(mv_transport_t t, char *s);
char *mv_transport_recv(mv_transport_t t);
int mv_transport_send_nb(mv_transport_t, char *s, reply_callback_t cb);
int mv_transport_recv_nb(mv_transport_t t, recv_callback_t cb);

mv_ttag_t mv_transport_tag();
const char *mv_transport_addr(mv_transport_t t);


#ifdef __cplusplus
}
#endif

#endif /* MV_TRANSPORT_H */
