/**
 * @file term.c
 *
 * @author Library for terminal I/O.
 */
#include <stdio.h>
#include <mv/value.h>


extern int sysprint(int arg)
{
  fprintf(stdout, "SYSPRINT: %d\n", arg);
}


extern 
