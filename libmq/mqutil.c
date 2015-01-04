/**
 * @file mqutil.c
 *
 * @author cjeong
 */
#include <stdlib.h>      /* free */
#include <string.h>      /* strdup */
#include <sys/socket.h>  /* inet_nota */
#include <sys/types.h>   /* getifaddrs */
#include <netinet/in.h>  /* sockaddr_in */
#include <arpa/inet.h>   /* inet_nota */
#include <ifaddrs.h>     /* getifaddrs */
#include <zmq.h>         /* zmq_ctx_new */

#include "mqutil.h"


#define MAX_MQSOCK_POOL 32
typedef struct _mqsock {
  void *sock;               /* sock */
  char *addr;               /* transport addr */

  unsigned free     : 1;    /* free or not */
  unsigned free_idx : 12;   /* index to free list */
  unsigned free_nxt : 12;   /* index of next free entry */
  unsigned pad      : 7;
} _mqsock_t;
static _mqsock_t _mqsock_pool[MAX_MQSOCK_POOL];
static _mqsock_t *_free_mqsock;

void _mqsock_pool_init()
{
  int i;
  _mqsock_t *sock = _mqsock_pool;

  for (i = 0; i < MAX_MQSOCK_POOL; i++) {
    sock->sock = NULL;
    sock->addr = NULL;

    sock->free = 1;
    sock->free_idx = i;
    sock->free_nxt = i + 1;
    sock++;
  }
  /* the last element */
  sock->free_idx = i;
  sock->free_nxt = 0;

  /* The first socket, _mqsock_pool[0], is reserved to represent "no more
     mqsock". If free_nxt == 0, it means no more free mqsock is available. */
  _free_mqsock = _mqsock_pool + 1;
  _mqsock_pool[0].free = 0;
}

_mqsock_t *_mqsock_get_free()
{
  if (_free_mqsock == _mqsock_pool) {
    /* if full, delete one, say mqsock_pool[1] */
    _mqsock_delete(_mqsock_pool[1]);
    return _mqsock_get_free();
  }

  _free_mqsock = _mqsock_pool + _free_mqsock->free_nxt;
  _free_mqsock->free = 0;

  return _free_mqsock;
}

int _mqsock_delete(_mqsock_t *mqsock)
{
  free(mqsock->addr);
  zmq_close(mqsock->sock);
  mqsock->free = 1;

  mqsock->free_nxt = mqsock->free_idx;
  _free_mqsock = mqsock;

  return 0;
}

_mqsock_t *_mqsock_lookup(const char *addr)
{
  /* TODO: hash table */
  int i;
  _mqsock_t *mqsock;
  for (i = 1; i < MAX_MQSOCK_POOL; i++) {
    mqsock = _mqsock_pool + i;
    if (!mqsock->free && !strcmp(mqsock->addr, addr))
      return mqsock;
  }

  return NULL;
  
}

/*
 * Functions for mqutil interface.
 */
static void *_zmq_ctx = NULL;
void mqutil_init()
{
  _zmq_ctx = zmq_ctx_new();
  _mqsock_pool_init();
}

void *mqutil_getsock(const char *addr)
{
  _mqsock_t *mqsock = _mqsock_lookup(addr);
  if (mqsock) {
    return mqsock->sock;
  }

  mqsock = _mqsock_get_free();

  if ((mqsock->sock = zmq_socket(_zmq_ctx, ZMQ_REQ)) == NULL) {
    perror("zmq_socket");
    _mqsock_delete(mqsock);
    return NULL;
  }

  if (zmq_connect(mqsock->sock, addr) != 0) {
    perror("zmq_connect");
    _mqsock_delete(mqsock);
    return NULL;
  }

  mqsock->addr = strdup(addr);
  return mqsock->sock;
}

const char *mqutil_getaddr()
{
  static char addr[1024];

  struct ifaddrs *ifaddr;
  struct ifaddrs *ifa;

  if (getifaddrs(&ifaddr) == -1) {
    perror("getifaddrs@mqutil_getaddr");
    return NULL;
  }

  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL)
      continue;
    
    if (ifa->ifa_addr->sa_family != AF_INET)
      continue;

    struct sockaddr_in *paddr = (struct sockaddr_in *) ifa->ifa_addr;
    if (strcmp(ifa->ifa_name, "eth0"))
      continue;
    strcpy(addr, inet_ntoa(paddr->sin_addr));
  }
  
  freeifaddrs(ifaddr);

  return &addr[0];
}
