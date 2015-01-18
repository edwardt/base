/**
 * @file main.c
 */
#include <stdio.h>      /* fprintf */
#include <stdlib.h>     /* exit */


extern int mvsh_repl();

int main(int args, char *argv[])
{

  /* initialize device service */
  mv_device_service_init("mvsh", "etc/device.dat");

  mv_message_setport(5558);
  
  mvsh_repl();

  return EXIT_SUCCESS;
}
