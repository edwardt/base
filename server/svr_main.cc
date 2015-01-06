/**
 * @file main.cc
 *
 * @brief Proxy for pub-sub network
 */
#include <stdio.h>            /* printf */
#include <unistd.h>           /* sysconf, sleep */
#include <stdlib.h>           /* EXIT_SUCCESS */
#include <sys/types.h>        /* waitpid */
#include <sys/wait.h>         /* waitpid */
#include <sys/stat.h>         /* waitpid */
#include <string.h>           /* strdup */
#include <fcntl.h>            /* O_RDWR */
#include <signal.h>           /* sigaction */
#include <zmq.h>              /* 0MQ */
#include <mq/mqueue.h>        /* mv_mqueue_t */
#include "svr_processor.hh"

using namespace mv::server;

static void sig_child(int signo)
{
  pid_t pid;
  int status;
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    printf("child %d terminated\n", pid);
 
  if (pid == -1) {
    /* child processes not fully cleaned up: possibility of zombies */
    perror("waitpid@sig_child");
  }
  return;
}


/* 
 * the main entry point
 */
int main(int argc, char *argv[])
{
  /* initialization for daemon */
  /*
     daemon_init(0);
     register_signal_handler(SIGCHLD, sig_child);
  */

  /*
    if (argc < 2) {
    fprintf(stdout, "Usage: %s [port]\n", argv[0]);
    exit(1);
    }
  */
  
  /* DEVICE DIRECTORY service: End devices can query if some device is 
     on-line and, if so, the transport address of the device. */
  mv_mqueue_t *mq = mv_mqueue_init(5555);
  MessageProcessor *mproc = MessageProcessor::getInstance(mq);

  mv_mqueue_run(mq);
  mproc->run();
  
  /* PUBSUB service: When end devices connect, broadcast to devices which
     subscribe to any of the "tag" which the connected device contains. */
  void *ctx = zmq_ctx_new();
  void *publisher = zmq_socket(ctx, ZMQ_PUB);
  zmq_bind(publisher, "tcp://*:5555");
  
  /* temporarily, generate timer event manually */
  while (1) {
    sleep(60);
  }

  return EXIT_SUCCESS;
}
