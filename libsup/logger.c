/**
 * @file logger.c
 *
 * @author cjeong
 */
#include <stdio.h>           /* vprintf */
#include <stdarg.h>          /* va_start */
#include "support/logger.h"


static int _verbosity;


/*
 * Functions for the logger interface.
 */
void mv_set_verbosity(mv_verbose_t v)
{
  _verbosity = v;
}

int mv_log(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  return 0;
}
