/**
 * @file transport_tcp.c
 */
#include <mv/transport.h>


typedef struct {
  mv_transtag_t tag;
  int (*send)(char *s);
  char (*recv)();
} _transport_t;


mv_tansport_t _tcp_open(const char *s)
{
}
