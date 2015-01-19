/**
 * @file main.c
 */
#include <stdio.h>      /* fprintf */
#include <stdlib.h>     /* exit */


extern int mvsh_repl();

int main(int args, char *argv[])
{

  /* Initialize device service. */
  mv_device_service_init("mvsh", "etc/device.dat");

  /* Set the message layer to use port 5558 to facilitate debugging on
     the same machine. Running the mvrt runtime and mvsh using the same
     port causes port conflict. */
  mv_message_setport(5558);

  /* Enter read-eval-print loop. */
  mvsh_repl();

  return EXIT_SUCCESS;
}
