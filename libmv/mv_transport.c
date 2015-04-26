/**
 * @file mv_transport.c
 */
#include <mv/transport.h>

static typedef struct {
  mv_transtag_t tag;
  int (*open)(const char *addr);
  int (*send)(char *s);
  char *(*recv)();
} _handler_t;

static typedef struct {
  mv_transtag_t tag;
  char *addr;
  _handler_t *handler;
} _transport_t;

static _handler_t _handlers[MV_TRANSPORT_NTAGS];

_transport_handler_t *_tansport_get_handler(mv_transtag_t tag)
{
  _handler_t *handler = _handlers + tag;
  if (handler->open)
    return handler;

  switch (tag) { 
  case MV_TRANSPORT_TCP: {
    return;
  }
  default:
    break;
  }
}

_transport_t *_transport_new(const char *s)
{
  _transport_t *trans = malloc(sizeof(_transport_t));
  if (!strncmp(s, "tcp://", 6)) {
    trans->tag = MV_TRANSPORT_TCP;
    handler = 
  }
}

int _transport_delete(_transport_t *trans)
{
  if (trans->addr)
    free(trans->addr);
}

/*
 * Transport API.
 */
mv_transport_t mv_transport_open(const char *s)
{
  _tansport_t *trans = _tranport_new(s);
}

int mv_transport_send(mv_transport_t t, char *s)
{
}

char *mv_transport_recv(mv_transport_t t)
{
}

mv_transtag_t mv_transport_tag()
{
}

const char *mv_transport_addr(mv_transport_t t)
{
}

