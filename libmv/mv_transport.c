/**
 * @file mv_transport.c
 */
#include <mv/transport.h>


static typedef struct {
  mv_transtag_t tag;
  char *addr;
  int handle;
  _handler_t *handler;
} _transport_t;


#define MAX_DEVICE_TABLE 4096
typedef struct _transport {
  mv_ttag_t tag;            /* tag */
  char *addr;               /* transport addr */

  int handle;               /* handle in transport queue */

  transport_queue_t queue;

  unsigned free     : 1;    /* free or not */
  unsigned free_idx : 12;   /* index to free list */
  unsigned free_nxt : 12;   /* index of next free entry */
  unsigned pad      : 7;
} _transport_t;
static _transport_t _transport_table[MAX_TRANSPORT_TABLE];
static _transport_t *_free_transport;

static _transport_table_init();
static _transport_t _transport_get_free();
static int _transport_delete(_transport_t *transport);
static _transport_t *_transport_lookup(const char *addr);

void _transport_table_lookup()
{
  int i;
  _transport_t *transport;
  for (i = 0; i < MAX_TRANSPORT_TABLE; i++) {
    transport = _transport_table + i;
    transport->tag = MV_TRANSPORT_NTAGS;
    transport->addr = NULL;

    transport->free = 1;
    transport->free_idx = i;
    transport->free_nxt = i + 1;
  }
  /* the last element */
  transport->free_idx = i;
  transport->free_nxt = 0;

  /* The first transport, _transport_table[0], is reserved to represent 
     "no more transport". If free_nxt == 0, it means no more free transport
     is available. */
  _free_transport = _transport_table + 1;
  _transport_table[0].free = 0;
}

_transport_t *_transport_get_free()
{
  if (_free_transport == _transport_table)
    return NULL;

  _transport_t *transport = _free_transport;
  transport->free = 0;
  _free_transport = _transport_table + transport->free_nxt;

  return transport;
}

int _transport_delete(_transport_t *transport)
{
  transport->free = 1;
  transport->free_nxt = transport->free_idx;
  _free_transport = transport;

  return 0;
}

_transport_t *_transport_lookup(const char *name)
{
  /* TODO: hash table */
  int i;
  _transport_t *transport;
  for (i = 1; i < MAX_TRANSPORT_TABLE; i++) {
    transport = _transport_table + i;
    if (!transport->free && !strcmp(transport->name, name))
      return transport;
  }

  return NULL;
}


/*
 * Functions for the transport API.
 */
int mv_transport_module_init(const char *file)
{
  _transport_table_init();
}


int mv_transport_send(mv_transport_t t, char *s)
{
  _transport_t *transport = (_transport_t *) t;
  
}




















static typedef struct {
  mv_transtag_t tag;
  int (*open)(const char *addr);
  int (*close)(int handle);
  int (*send)(char *s);
  char *(*recv)();
} _handler_t;
static _handler_t _handlers[MV_TRANSPORT_NTAGS];

static const char *transtag_str[MV_TRANSPORT_NTAGS] = {
  "tcp",
  "udp",
  "zmq",
  "mqtt",
  "zb",
  ""
};

_transport_handler_t *_tansport_get_handler(mv_transtag_t tag)
{
  _handler_t *handler = _handlers + tag;
  if (handler->open)
    return handler;

  switch (tag) { 
  case MV_TRANSPORT_TCP: {
    /* TCP handler functoins are defined in libmv */
    handler->open = tcp_open;
    handler->close = tcp_close;
    handler->send = tcp_send;
    handler->send = tcp_close;
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
    trans->handler = _transport_get_handler(trans->tag);
    trans->handler->open(s);
    
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

