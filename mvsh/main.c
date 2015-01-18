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
  
  mvsh_repl();

  return EXIT_SUCCESS;
}
