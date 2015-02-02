/**
 * @file mv_sendrecv_sock.c
 *
 * @brief Implementation of sendrecv functions using Unix sockets.
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
#include <netinet/in.h>  /* struct sockaddr_in */
#include <arpa/inet.h>   /* inet_nota */
#include <ifaddrs.h>     /* getifaddrs */
#include <netdb.h>       /* getaddrinfo */
#include <mv/device.h>   /* mv_device_self */
#include <mv/message.h>  /* mv_message_send */
#include "mv_netutil.h"  /* mv_writemsg */

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


/* 
 * Message layer info structure.
 */
typedef struct _mqinfo {
  char *addr;                   /* address string for input queue */
  char *srcstr;                 /* {"dev": "mydev", "addr": "..."} */

  int listenfd;                 /* REP (server) listen socket */

  pthread_t thr_rep;            /* REP (server) thread for input queue */
  pthread_t thr_req;            /* REQ (client) thread for output queue */

  _mq_t *imq;                   /* input message qeueue */
  _mq_t *omq;                   /* output message qeueue */
} _mqinfo_t;
static _mqinfo_t *_mqinfo_init(unsigned port);
static _mqinfo_t *_mqinfo_get();
static int _mqinfo_run(_mqinfo_t *mqinfo);


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

  struct timespec ts;                 /* time for nanosleep */
  struct sockaddr_storage claddr;     /* addr */
  socklen_t addrlen;                  /* addr length */
  int connfd;                         /* connected descriptor */
  int recvsz;                         /* size of received message */
  char *recvbuf;                      /* pointer to received message */
  char *recvstr;                      /* copy of received message */

  ts.tv_sec = 0;
  ts.tv_nsec = 1000;
  addrlen = sizeof(struct sockaddr_storage);

  while (1) {

    printf("accept\n");
    if ((connfd = accept(mq->listenfd, (SA *) &claddr, &addrlen)) == -1) {
      perror("accept@_mq_input_thread");
      continue;
    }

#if 0
#   define CLIADDRLEN (NI_MAXHOST + NI_MAXSERV + 10)
    char cliaddr_s[CLIADDRLEN];         /* client addrress */
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    if (getnameinfo((SA *) & claddr, addrlen, host, NI_MAXHOST, service,
                    NI_MAXSERV, 0) == 0)
      snprintf(cliaddr_s, CLIADDRLEN, "(%s, %s)", host, service);
    else
      snprintf(cliaddr_s, CLIADDRLEN, "(UNKNOWN)");
    fprintf(stdout, "Connection from %s\n", cliaddr_s);
#endif

    printf("read\n");
    if ((recvsz = mv_readmsg(connfd, &recvbuf)) == -1) {
      perror("read@_mq_input_thread");
      continue;
    }
     
    recvstr = malloc(recvsz + 1);
    memcpy(recvstr, (char *) recvbuf, recvsz);
    recvstr[recvsz] = '\0';

#if 0
    fprintf(stdout, "Message received: [%s]:%d\n", recvstr, recvsz);
#endif
    
    while (_mq_enqueue(mq->imq, recvstr) != 0) {
      nanosleep(&ts, NULL);
    }

    if (close(connfd) == -1) {
      perror("close@_mq_input_thread");
      continue;
    }
  }

  pthread_exit(NULL);
}

void *_mq_output_thread(void *arg)
{
  _mqinfo_t *mq = (_mqinfo_t *) arg;  /* message queue */

  struct timespec ts;                 /* time for nanosleep */
  struct sockaddr_in servaddr;        /* server addr */
  char *sendstr;                      /* message to send */

  struct addrinfo hints;              /* addrinfo */
  struct addrinfo *result, *rp;       /* getaddrinfo results */
  int connfd;                         /* connected description */
  int rv;                             /* returnv value */

  ts.tv_sec = 0;
  ts.tv_nsec = 1000;

  while (1) {

    if ((sendstr = _mq_dequeue(mq->omq)) == NULL) {
      nanosleep(&ts, NULL);
      continue;
    }

    const char * sendaddr = _mq_getaddr(sendstr);
    const char *senddata = _mq_getdata(sendstr);
    char *addr = strstr(sendaddr, "//") + 2;
    char *port = strdup(strstr(addr, ":") + 1);
    port[strlen(port)] = '\0';
    char *ipaddr = strdup(addr);
    ipaddr[port-addr] = '\0';
#if 1
    fprintf(stdout, "Message to [%s]:[%s]: %s\n", ipaddr, port, senddata);
#endif

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    if ((rv = getaddrinfo(ipaddr, port, &hints, &result)) != 0) {
      fprintf(stderr, "getaddrinfo@_mq_output_thread: %s at %s:%s\n", 
              gai_strerror(rv), ipaddr, port);
      continue;
    }

    /* walk through returned list until we find an address structure that
       can be used to succeffully connect a socket */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
      connfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
      if (connfd == -1)
        continue;

      if (connect(connfd, rp->ai_addr, rp->ai_addrlen) != -1)
        break;

      close(connfd);
    }

    if (rp == NULL) {
      fprintf(stderr, "Failed to connect socket to any address.\n");
      continue;
    }
    freeaddrinfo(result);

    mv_writemsg(connfd, senddata);

    close(connfd);
    free(sendstr);
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

#define BACKLOG 128
_mqinfo_t *_mqinfo_init(unsigned port)
{
  _mqinfo_t *mq = malloc(sizeof(_mqinfo_t));
  mq->imq = _mq_new(MAX_MESSAGE_QUEUE);
  mq->omq = _mq_new(MAX_MESSAGE_QUEUE);

  char port_s[128];                   /* port */
  struct addrinfo hints;              /* addrinfo */
  struct addrinfo *result, *rp;       /* getaddrinfo results */
  int rv;                             /* return value */
  int reuseaddr;

  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
    fprintf(stderr, "_mqinfo_init: Failed to ignore SIGPIPE.\n");
    exit(1);
  }

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV | AI_ADDRCONFIG;
  snprintf(port_s, 127, "%d", port);
  if ((rv = getaddrinfo(NULL, port_s, &hints, &result)) != 0) {
    fprintf(stderr, "getaddrinfo@_mqinfo_init: %s\n", gai_strerror(rv));
    exit(1);
  }
  
  /* walk through returned list until we find an address structure that
     can be used to succeffully create and bind a socket */
  reuseaddr = 1;
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    mq->listenfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (mq->listenfd == -1)
      continue;

    if (setsockopt(mq->listenfd, SOL_SOCKET, SO_REUSEADDR, 
                   &reuseaddr, sizeof(reuseaddr)) == -1) {
      perror("setsockopt@_mqinfo_init");
      exit(1);
    }

    if (bind(mq->listenfd, rp->ai_addr, rp->ai_addrlen) == 0)
      break;

    close(mq->listenfd);
  }

  if (rp == NULL) {
    fprintf(stderr, "Failed to bind a socket to any address.\n");
    exit(1);
  }

  if (listen(mq->listenfd, SOMAXCONN) == -1) {
    perror("listen@_mqinfo_init");
    exit(1);
  }

  freeaddrinfo(result);

  mq->addr = strdup(_mq_selfaddr());
  
  return mq;
}

int _mqinfo_run(_mqinfo_t *mqinfo) 
{
  /* SIGRTMIN will be used by runtime in interval timers. Block this
     signal in mq threads. */
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
    /* create mqinfo and initialize the queues when this function
       is first called. */
    if ((_mqinfo = _mqinfo_init(_mqport)) == NULL)
      return NULL;

    if (_mqinfo_run(_mqinfo) == -1)
      return NULL;
  }

  return _mqinfo;
}

/*
 * Implementation of send and recv functions.
 */
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
  if (_mqinfo) {
    fprintf(stderr, "Message queue already created. Call mv_message_setport "
            "before any mv_message_send/recv calls.\n");
    exit(1);
  }
  _mqport = port;
  return 0;
}

