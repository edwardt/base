/**
 * @file sendrecv_zmq.c
 *
 * @brief Implementation of sendrecv functions using ZeroMQ.
 */
#include <stdio.h>       /* sprintf */
#include <stdlib.h>      /* malloc */
#include <string.h>      /* memcpy */
#include <time.h>        /* nanosleep */
#include <errno.h>       /* errno */
#include <pthread.h>     /* pthread_create */
#include <signal.h>      /* sigemptyset */
#include <assert.h>      /* assert */
#include <sys/socket.h>  /* inet_nota */
#include <sys/types.h>   /* getifaddrs */
#include <netinet/in.h>  /* sockaddr_in */
#include <arpa/inet.h>   /* inet_nota */
#include <ifaddrs.h>     /* getifaddrs */
#include <zmq.h>         /* zmq_ctx_new */
#include <mv/device.h>   /* mv_device_self */
#include <mv/message.h>


#define MAX_MESSAGE_QUEUE 4096
typedef struct _mq {
  pthread_mutex_t lock;
  int size;
  int head;
  int tail;
  char **msgs;
} _mq_t;

static _mq_t *_mq_new(int size);
static int _mq_delete(_mq_t *mq);
static int _mq_enqueue(_mq_t *mq, char *s);
static char *_mq_dequeue(_mq_t *mq);
static void *_mq_input_thread(void *arg);
static void *_mq_output_thread(void *arg);
static const char *_mq_selfaddr();
static const char *_mq_getaddr(const char *str);
static const char *_mq_getdata(const char *str);


typedef struct _mqinfo {
  char *addr;          /* address for input queue */
  char *srcstr;        /* {"dev": "mydev", "addr": "..."} */

  void *ctx;           /* zmq context */
  void *sock;          /* REP socket */

  pthread_t thr_rep;   /* REP thread for input queue */
  pthread_t thr_req;   /* REQ thread for output queue */

  _mq_t *imq;          /* input message qeueue */
  _mq_t *omq;          /* output message qeueue */
} _mqinfo_t;
static _mqinfo_t *_mqinfo_init(unsigned port);
static _mqinfo_t *_mqinfo_get();
static int _mqinfo_run(_mqinfo_t *mqinfo);


#define MAX_MQSOCK_POOL 32
typedef struct _sock {
  void *sock;               /* sock */
  char *addr;               /* transport addr */

  unsigned free     : 1;    /* free or not */
  unsigned free_idx : 12;   /* index to free list */
  unsigned free_nxt : 12;   /* index of next free entry */
  unsigned pad      : 7;
} _sock_t;
static _sock_t _sockpool[MAX_MQSOCK_POOL];
static _sock_t *_free_sock;

static void _sockpool_init();
static _sock_t *_sock_getfree();
static int _sock_delete(_sock_t *sock);
static _sock_t *_sockpool_lookup(const char *addr);
static void *_sockpool_getsock(const char *addr, void *ctx);


_mq_t *_mq_new(int size)
{
  if (size > MAX_MESSAGE_QUEUE) {
    fprintf(stdout, "Max message queue size is %d.\n", MAX_MESSAGE_QUEUE);
    size = MAX_MESSAGE_QUEUE;
  }
  _mq_t *mq = malloc(sizeof(_mq_t));
  pthread_mutex_init(&mq->lock, NULL);
  mq->size = size;
  mq->head = 0;
  mq->tail = 0;
  mq->msgs = malloc(sizeof(char *) * size);

  return mq;
}

int _mq_delete(_mq_t *mq)
{
  free(mq->msgs);
  free(mq);

  return 0;
}

int _mq_full(_mq_t *mq)
{
  return (mq->tail + 1) % mq->size == mq->head;
}

int _mq_empty(_mq_t *mq)
{
  return mq->tail == mq->head;
}

int _mq_enqueue(_mq_t *mq, char *s)
{
  if (pthread_mutex_lock(&mq->lock) != 0) {
    perror("pthread_mutex_lock@_mq_enqueue");
    return -1;
  }

  if (_mq_full(mq)) {
    if (pthread_mutex_unlock(&mq->lock) != 0) {
      perror("pthread_mutex_unlock@_mq_enqueue");
    }
    return -1;
  }

  mq->msgs[mq->tail] = s;
  mq->tail = (mq->tail + 1) % mq->size;

  if (pthread_mutex_unlock(&mq->lock) != 0)  {
    perror("pthread_mutex_unlock@_mq_enqueue");
    return -1;
  }

  return 0;
}

char *_mq_dequeue(_mq_t *mq)
{
  pthread_mutex_lock(&mq->lock);
  if (_mq_empty(mq)) {
    pthread_mutex_unlock(&mq->lock);
    return NULL;
  }

  char *s = mq->msgs[mq->head];
  mq->head = (mq->head + 1) % mq->size;

  pthread_mutex_unlock(&mq->lock);
  return s;
}

void *_mq_input_thread(void *arg)
{
  _mqinfo_t *mq = (_mqinfo_t *) arg;  /* message queue */

  int recvsz;                         /* msg size from zmq_msg_recv */
  char *recvstr;                      /* msg str from zmq_msg_recv */
  int sendsz;                         /* msg size for zmq_msg_send */
  char sendstr[1024];                 /* msg str for zmq_msg_send */
  struct timespec ts;                 /* time for nanosleep */

  ts.tv_sec = 0;
  ts.tv_nsec = 1000;

  zmq_pollitem_t items[] = {
    { mq->sock, 0, ZMQ_POLLIN, 0 },
  };
  int nitems = sizeof(items)/sizeof(items[0]);

  while (1) {
    zmq_msg_t recvmsg;
    zmq_msg_t sendmsg;
    zmq_msg_init(&recvmsg);
    zmq_msg_init(&sendmsg);

    /* 
       printf("Poll\n");
       zmq_poll(items, nitems, -1);

       if (!(items[0].revents & ZMQ_POLLIN))
       continue;
    */

    if ((recvsz = zmq_msg_recv(&recvmsg, mq->sock, 0)) == -1) {
      if (errno == EAGAIN) {
        zmq_msg_close(&recvmsg);
        zmq_msg_close(&sendmsg);
        continue;
      }

      perror("zmq_msg_recv@_mq_input_thread");
      exit(1);
    }

    /* str must be freed when _mq_dequeued */
    recvstr = malloc(recvsz + 1);
    memcpy(recvstr, (char *) zmq_msg_data(&recvmsg), recvsz);
    zmq_msg_close(&recvmsg);
    recvstr[recvsz] = '\0';

    /*
      fprintf(stdout, "Message received: [%s]:%d\n", recvstr, recvsz);
    */

    while (_mq_enqueue(mq->imq, recvstr) != 0) {
      nanosleep(&ts, NULL);
    }

    /* ACK - optimize away this later */
    strcpy(sendstr, "REPLY");
    sendsz = strlen(sendstr);
    zmq_msg_init_size(&sendmsg, sendsz);
    memcpy(zmq_msg_data(&sendmsg), sendstr, sendsz);
    if (zmq_msg_send(&sendmsg, mq->sock, 0) == -1) {
      perror("zmg_msg_send@_mq_input_thread");
      exit(1);
    }

    zmq_msg_close(&recvmsg);
    zmq_msg_close(&sendmsg);
  }

  pthread_exit(NULL);
}

void *_mq_output_thread(void *arg)
{
  _mqinfo_t *mq = (_mqinfo_t *) arg;  /* message queue */

  char *recvstr;                      /* msg str from zmq_msg_recv */
  char *sendstr;                      /* msg str for zmq_msg_send */
  struct timespec ts;                 /* time for nanosleep */
  ts.tv_sec = 0;
  ts.tv_nsec = 1000;

  char *smsg;

  while (1) {
    if ((sendstr = _mq_dequeue(mq->omq)) == NULL) {
      nanosleep(&ts, NULL);
      continue;
    }

    printf("sendstr:%s\n", sendstr);
    const char *sendaddr = _mq_getaddr(sendstr);
    const char *senddata = _mq_getdata(sendstr);

    void *sendsock = _sockpool_getsock(sendaddr, mq->ctx);
    if (sendsock == NULL) 
      continue;

    zmq_msg_t sendmsg;
    int sendsz = strlen(senddata);
    zmq_msg_init_size(&sendmsg, sendsz);
    memcpy(zmq_msg_data(&sendmsg), senddata, sendsz);
    int sz;
    if ((sz = zmq_msg_send(&sendmsg, sendsock, 0)) == -1) {
      perror("zmq_msg_send@_mq_output_thread");
      zmq_msg_close(&sendmsg);
      continue;
    }
    zmq_msg_close(&sendmsg);

    zmq_msg_t recvmsg;
    int recvsz;
    zmq_msg_init(&recvmsg);
    if ((recvsz = zmq_msg_recv(&recvmsg, sendsock, ZMQ_DONTWAIT)) != -1) {
      char *recvstr = malloc(recvsz + 1);
      memcpy(recvstr, (char *) zmq_msg_data(&recvmsg), recvsz);
      recvstr[recvsz] = '\0';
    }
  }
}

const char *_mq_selfaddr()
{
  static char addr_eth[1024];
  static char addr_wlan[1024];

  struct ifaddrs *ifaddr;
  struct ifaddrs *ifa;

  if (getifaddrs(&ifaddr) == -1) {
    perror("getifaddrs@_mq_selfaddr");
    return NULL;
  }

  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL)
      continue;
    
    if (ifa->ifa_addr->sa_family != AF_INET)
      continue;

    struct sockaddr_in *paddr = (struct sockaddr_in *) ifa->ifa_addr;
    if (!strcmp(ifa->ifa_name, "eth0"))
      strcpy(addr_eth, inet_ntoa(paddr->sin_addr));
    if (!strcmp(ifa->ifa_name, "wlan0"))
      strcpy(addr_wlan, inet_ntoa(paddr->sin_addr));
      
  }
  
  freeifaddrs(ifaddr);

  if (strcmp(addr_eth, ""))
    return &addr_eth[0];

  if (strcmp(addr_wlan, ""))
    return &addr_wlan[0];

  return NULL;
}

const char *_mq_getaddr(const char *str)
{
  static char addr[1024];
  char *data = strstr(str, "{");

  unsigned len = (unsigned long) data - (unsigned long) str;
  strncpy(addr, str, len);
  addr[len] = '\0';

  return &addr[0];
}

const char *_mq_getdata(const char *str)
{
  return strstr(str, "{");
}

_mqinfo_t *_mqinfo_init(unsigned port)
{
  _mqinfo_t *mqinfo = malloc(sizeof(_mqinfo_t));
  mqinfo->imq = _mq_new(MAX_MESSAGE_QUEUE);
  mqinfo->omq = _mq_new(MAX_MESSAGE_QUEUE);

  char s[1024];
  const char *selfaddr = _mq_selfaddr();
  if (!selfaddr) {
    fprintf(stderr, "_mqinfo_init: Failed to get the IP address of host.\n");
    exit(1);
  }
  
  sprintf(s, "tcp://%s:%d", _mq_selfaddr(), port);
  mqinfo->addr = strdup(s);

  const char *dev_s = mv_device_self();
  sprintf(s, "{\"dev\":\"%s\", \"addr\":\"%s\"}", dev_s, mqinfo->addr);
  mqinfo->srcstr = strdup(s);

  /* create ZMQ context */
  mqinfo->ctx = zmq_ctx_new();

  /* create socket for receiving requests */
  if ((mqinfo->sock = zmq_socket(mqinfo->ctx, ZMQ_REP)) == NULL) {
    perror("zmq_socket@_mqinfo_init");
    exit(1);
  }
  if (zmq_bind(mqinfo->sock, mqinfo->addr) == -1) {
    perror("zmq_bind@_mqinfo_init");
    exit(1);
  }

  /* initialize socket pool for sending requests */
  _sockpool_init(mqinfo->ctx);

  return mqinfo;
}

int _mqinfo_run(_mqinfo_t *mqinfo) 
{
  /* SIGRTMIN will be used by runtime in interval timers. Blcok this
     signal in any thread. */
  sigset_t sigmask;
  sigemptyset(&sigmask);
  sigaddset(&sigmask, SIGRTMIN);
  if (pthread_sigmask(SIG_BLOCK, &sigmask, NULL) != 0) {
    perror("pthread_sigmask@_mqinfo_run");
    return -1;
  }
  
  if (pthread_create(&mqinfo->thr_rep, NULL, _mq_input_thread, mqinfo) != 0) {
    perror("pthread_create@_mqinfo_run");
    return -1;
  }

  if (pthread_create(&mqinfo->thr_req, NULL, _mq_output_thread, mqinfo) != 0) {
    perror("pthread_create@m_mqinfo_run");
    return -1;
  }

  return 0;
}

#define ZMQ_DEFAULT_PORT 5557
static unsigned _mqport = ZMQ_DEFAULT_PORT;
static _mqinfo_t *_mqinfo = NULL;
_mqinfo_t *_mqinfo_get()
{
  if (!_mqinfo) {
    /* create an mqinfo and initialize the queues when this function
       is first called. */
    if ((_mqinfo = _mqinfo_init(_mqport)) == NULL)
      return NULL;

    if (_mqinfo_run(_mqinfo) == -1)
      return NULL;
  }

  return _mqinfo;
}



void _sockpool_init()
{
  int i;
  _sock_t *sock = &_sockpool[0];

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
  _free_sock = _sockpool + 1;
  _sockpool[0].free = 0;
}

_sock_t *_sock_getfree()
{
  if (_free_sock == _sockpool) {
    /* if full, delete one, say mqsock_pool[1] */
    _sock_delete(&_sockpool[1]);
    return _sock_getfree();
  }

  _free_sock = _sockpool + _free_sock->free_nxt;
  _free_sock->free = 0;

  return _free_sock;
}

int _sock_delete(_sock_t *sock)
{
  free(sock->addr);
  zmq_close(sock->sock);
  sock->free = 1;

  sock->free_nxt = sock->free_idx;
  _free_sock = sock;

  return 0;
}

_sock_t *_sockpool_lookup(const char *addr)
{
  /* TODO: hash table */
  int i;
  _sock_t *sock;
  for (i = 1; i < MAX_MQSOCK_POOL; i++) {
    sock = _sockpool + i;
    if (!sock->free && !strcmp(sock->addr, addr))
      return sock;
  }

  return NULL;
}

void *_sockpool_getsock(const char *addr, void *ctx)
{
  _sock_t *sock = NULL;
  /*
    FIX: For now, just create socket every time we send message.
    mqsock = _mqsock_lookup(addr);
    if (mqsock) {
    return mqsock->sock;
    }
  */

  if ((sock = _sock_getfree() ) == NULL)
    return NULL;

  if ((sock->sock = zmq_socket(ctx, ZMQ_REQ)) == NULL) {
    perror("zmq_socket@_sockpool_getsock");
    _sock_delete(sock);
    return NULL;
  }

  if (zmq_connect(sock->sock, addr) != 0) {
    perror("zmq_connect@_sockpool_getsock");
    _sock_delete(sock);
    return NULL;
  }

  sock->addr = strdup(addr);
  return sock->sock;
}


int mv_message_send(const char *adr, mv_mtag_t tag, char *arg_s)
{
  _mqinfo_t *mqinfo = _mqinfo_get();
  if (!mqinfo)
    return -1;

  const char *tag_s = mv_message_tagstr(tag);
  char *src_s = _mqinfo->srcstr;
  int sz = strlen(arg_s);
  char *m = malloc(sz + 1024);
  sprintf(m, "%s {\"tag\":\"%s\", \"arg\":%s, \"src\":%s}", 
          adr, tag_s, arg_s, src_s);

  while (_mq_enqueue(mqinfo->omq, m) != 0) ;

  return 0;
}

/*
 * Implementation of send and recv functions.
 */
int mv_message_send_value(const char *adr, mv_mtag_t tag, mv_value_t arg)
{
  _mqinfo_t *mqinfo = _mqinfo_get();
  if (!mqinfo)
    return -1;

  const char *tag_s = mv_message_tagstr(tag);
  char *arg_s = mv_value_to_str(arg);
  char *src_s = _mqinfo->srcstr;
  int sz = strlen(arg_s);
  char *m = malloc(sz + 1024);
  sprintf(m, "%s {\"tag\":%s,\"arg\":%s,\"src\":%s}", adr, tag_s, arg_s, src_s);
  
  while (_mq_enqueue(mqinfo->omq, m) != 0) ;

  return 0;
}

char *mv_message_recv()
{
  _mqinfo_t *mqinfo = _mqinfo_get();
  if (!mqinfo)
    return NULL;

  char *s = NULL;
  while ((s = _mq_dequeue(mqinfo->imq)) == NULL) ;

  return s;
}

const char *mv_message_selfaddr()
{
  _mqinfo_t *mq = _mqinfo_get();

  return mq->addr;
}

int mv_message_setport(unsigned port)
{
  _mqport = port;
  return 0;
}
