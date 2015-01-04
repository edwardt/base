/**
 * @file main.c
 */
#include <stdio.h>      /* fprintf */
#include <stdlib.h>     /* exit */
#include <mq/mqueue.h>  /* mv_mqueue_t */
#include "repl.h"       /* mvsh_repl */

int main(int args, char *argv[])
{

  /* initialize device service */
  mv_device_service_init("mvsh", "etc/device.dat");
  
  mv_mqueue_t *mq = mv_mqueue_init(DEFAULT_PORT+1);
  if (mv_mqueue_run(mq) == -1) {
    fprintf(stderr, "mv_mqueue_run: failed.\n");
    exit(1);
  }

  mvsh_repl(mq);

  return EXIT_SUCCESS;
}
