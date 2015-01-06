/**
 * @file rtlogger.c
 *
 * @author cjeong
 */
#include <stdio.h>           /* vprintf */
#include <stdarg.h>          /* va_start */
#include "rtlogger.h"


static int _verbosity;

/*
 * Functions for the logger interface.
 */
void mvrt_set_verbosity(mv_verbose_t v)
{
  _verbosity = v;
}

int mvrt_log(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

  return 0;
}
