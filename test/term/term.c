#include <stdio.h>

int foo(int arg)
{
  fprintf(stdout, "hello, world (ARG = %d)\n", arg);
}

int sysprint(int arg)
{
  fprintf(stdout, "SYSPRINT: %d\n", arg);
}
