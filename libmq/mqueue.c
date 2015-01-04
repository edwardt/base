/**
 * @file mqueue.c
 *
 * @author cjeong
 */
#include <stdio.h>       /* sprintf */
#include <stdlib.h>      /* malloc */
#include <string.h>      /* memcpy */
#include <time.h>        /* nanosleep */
#include <errno.h>       /* errno */
#include <pthread.h>     /* pthread_create */
#include <signal.h>      /* sigemptyset */
#include <assert.h>      /* assert */
#include <zmq.h>         /* zmq_ctx_new */
#include <mq/mqueue.h>
#include "mqutil.h"


/*
 * ZMQ-based message queue implementation.
 */
#define MAX_MESSAGE_QUEUE 4096
typedef struct _mqueue {
  pthread_mutex_t lock;
  int size;
  int head;
  int tail;
  char **msgs;
} _mqueue_t;

typedef struct _mqinfo {
  char *addr;          /* address for input queue */

  void *ctx;           /* zmq context */
  void *sock;          /* REP socket */

  pthread_t thr_rep;   /* REP thread for input queue */
  pthread_t thr_req;   /* REQ thread for output queue */

  _mqueue_t *imq;      /* input message qeueue */
  _mqueue_t *omq;      /* output message qeueue */
} _mqinfo_t;

static _mqueue_t *_mqueue_new(int size);
static int _mqueue_delete(_mqueue_t *mq);
static int _mqueue_enqueue(_mqueue_t *mq, char *s);
static char *_mqueue_dequeue(_mqueue_t *mq);
static void *_mqueue_input_thread(void *arg);
static void *_mqueue_output_thread(void *arg);
static const char *_mqueue_getaddr(const char *str);
static const char *_mqueue_getdata(const char *str);

_mqueue_t *_mqueue_new(int size)
{
  if (size > MAX_MESSAGE_QUEUE) {
    fprintf(stdout, "Max message queue size is %d.\n", MAX_MESSAGE_QUEUE);
    size = MAX_MESSAGE_QUEUE;
  }
  _mqueue_t *mq = malloc(sizeof(_mqueue_t));
  pthread_mutex_init(&mq->lock, NULL);
  mq->size = size;
  mq->head = 0;
  mq->tail = 0;
  mq->msgs = malloc(sizeof(char *) * size);

  return mq;
}

int _mqueue_delete(_mqueue_t *mq)
{
  free(mq->msgs);
  free(mq);

  return 0;
}

int _mqueue_full(_mqueue_t *mq)
{
  return (mq->tail + 1) % mq->size == mq->head;
}

int _mqueue_empty(_mqueue_t *mq)
{
  return mq->tail == mq->head;
}

int _mqueue_enqueue(_mqueue_t *mq, char *s)
{
  if (pthread_mutex_lock(&mq->lock) != 0) {
    perror("pthread_mutex_lock@_mqueue_enqueue");
    return -1;
  }

  if (_mqueue_full(mq)) {
    if (pthread_mutex_unlock(&mq->lock) != 0) {
      perror("pthread_mutex_unlock@_mqueue_enqueue");
    }
    return -1;
  }

  /*
    fprintf(stdout, "_mqueue_enqueue: %s\n", s);
  */

  mq->msgs[mq->tail] = s;
  mq->tail = (mq->tail + 1) % mq->size;

  if (pthread_mutex_unlock(&mq->lock) != 0)  {
    perror("pthread_mutex_unlock@_mqueue_enqueue");
    return -1;
  }

  return 0;
}

char *_mqueue_dequeue(_mqueue_t *mq)
{
  pthread_mutex_lock(&mq->lock);
  if (_mqueue_empty(mq)) {
    pthread_mutex_unlock(&mq->lock);
    return NULL;
  }

  char *s = mq->msgs[mq->head];
  mq->head = (mq->head + 1) % mq->size;

  /*
    fprintf(stdout, "_mqueue_dequeue: %s\n", s);
  */

  pthread_mutex_unlock(&mq->lock);
  return s;
}

void *_mqueue_input_thread(void *arg)
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

      perror("zmq_msg_recv@_mqueue_input_thread");
      exit(1);
    }

    /* str must be freed when _mqueue_dequeued */
    recvstr = malloc(recvsz + 1);
    memcpy(recvstr, (char *) zmq_msg_data(&recvmsg), recvsz);
    zmq_msg_close(&recvmsg);
    recvstr[recvsz] = '\0';

    /*
      fprintf(stdout, "Message received: [%s]:%d\n", recvstr, recvsz);
    */

    while (_mqueue_enqueue(mq->imq, recvstr) != 0) {
      nanosleep(&ts, NULL);
    }

    /* ACK - optimize away this later */
    strcpy(sendstr, "REPLY");
    sendsz = strlen(sendstr);
    zmq_msg_init_size(&sendmsg, sendsz);
    memcpy(zmq_msg_data(&sendmsg), sendstr, sendsz);
    if (zmq_msg_send(&sendmsg, mq->sock, 0) == -1) {
      perror("zmg_msg_send@_mqueue_input_thread");
      exit(1);
    }

    zmq_msg_close(&recvmsg);
    zmq_msg_close(&sendmsg);
  }

  pthread_exit(NULL);
}

void *_mqueue_output_thread(void *arg)
{
  _mqinfo_t *mq = (_mqinfo_t *) arg;  /* message queue */

  char *recvstr;                      /* msg str from zmq_msg_recv */
  char *sendstr;                      /* msg str for zmq_msg_send */
  struct timespec ts;                 /* time for nanosleep */
  ts.tv_sec = 0;
  ts.tv_nsec = 1000;

  char *smsg;

  while (1) {
    if ((sendstr = _mqueue_dequeue(mq->omq)) == NULL) {
      nanosleep(&ts, NULL);
      continue;
    }

    const char *sendaddr = _mqueue_getaddr(sendstr);
    const char *senddata = _mqueue_getdata(sendstr);

    void *sendsock = mqutil_getsock(sendaddr);
    if (sendsock == NULL) 
      continue;

    zmq_msg_t sendmsg;
    int sendsz = strlen(senddata);
    zmq_msg_init_size(&sendmsg, sendsz);
    memcpy(zmq_msg_data(&sendmsg), senddata, sendsz);
    int sz;
    if ((sz = zmq_msg_send(&sendmsg, sendsock, 0)) == -1) {
      perror("zmq_msg_send@_mqueue_output_thread");
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

const char *_mqueue_getaddr(const char *str)
{
  static char addr[1024];
  char *data = strstr(str, "{");

  size_t len = (unsigned long) data - (unsigned long) str;
  strncpy(addr, str, len);
  addr[len] = '\0';

  return &addr[0];
}

const char *_mqueue_getdata(const char *str)
{
  return strstr(str, "{");
}

/*
 * Functions for the mqueue interface.
 */
mv_mqueue_t *mv_mqueue_init(unsigned port)
{
  _mqinfo_t *mq = malloc(sizeof(_mqinfo_t));
  mq->imq = _mqueue_new(MAX_MESSAGE_QUEUE);
  mq->omq = _mqueue_new(MAX_MESSAGE_QUEUE);

  char addr[1024];
  sprintf(addr, "tcp://%s:%d", mqutil_getaddr(), port);

  /* initialize mqutil */
  mqutil_init();

  mq->addr = strdup(addr);
  mq->ctx = zmq_ctx_new();

  /* create socket for receiving requests */
  if ((mq->sock = zmq_socket(mq->ctx, ZMQ_REP)) == NULL) {
    perror("zmq_socket@mv_mqueue_new");
    exit(1);
  }
  if (zmq_bind(mq->sock, addr) == -1) {
    perror("zmq_bind@mv_mqueue_new");
    exit(1);
  }

  return (mv_mqueue_t *) mq;
}

int mv_delete_new(mv_mqueue_t *q)
{
  _mqueue_t *mq = (_mqueue_t *) q;

  return _mqueue_delete(mq);
}

int mv_mqueue_run(mv_mqueue_t *q) 
{
  _mqinfo_t *mq = (_mqinfo_t *) q;

  /* SIGRTMIN will be used by runtime in interval timers. Blcok this
     signal in any thread. */
  sigset_t sigmask;
  sigemptyset(&sigmask);
  sigaddset(&sigmask, SIGRTMIN);
  if (pthread_sigmask(SIG_BLOCK, &sigmask, NULL) != 0) {
    perror("pthread_sigmask@mv_mqueue_run");
    return -1;
  }
  
  if (pthread_create(&mq->thr_rep, NULL, _mqueue_input_thread, mq) != 0) {
    perror("pthread_create@mv_mqueue_run");
    return -1;
  }

  if (pthread_create(&mq->thr_req, NULL, _mqueue_output_thread, mq) != 0) {
    perror("pthread_create@mv_mqueue_run");
    return -1;
  }

  return 0;
}

char *mv_mqueue_get(mv_mqueue_t *q)
{
  if (!q)
    return NULL;

  _mqinfo_t *mq = (_mqinfo_t *) q;

  char *s = NULL;
  while ((s = _mqueue_dequeue(mq->imq)) == NULL) ;
  return s;

  //  return _mqueue_dequeue(mq->imq);
}

int mv_mqueue_put(mv_mqueue_t *q, char *msg)
{
  if (!q)
    return -1;

  _mqinfo_t *mq = (_mqinfo_t *) q;

  while (_mqueue_enqueue(mq->omq, msg) != 0) ;
  //return _mqueue_enqueue(mq->omq, msg);

  return 0;
}

const char *mv_mqueue_addr(mv_mqueue_t *q)
{
  if (!q)
    return 0;

  _mqinfo_t *mq = (_mqinfo_t *) q;

  return mq->addr;
}
