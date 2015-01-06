/**
 * @file rterror.c
 *
 * @author cjeong
 */
#include <assert.h>
#include "rterror.h"

#define MAX_TRACED_STACK_FRAMES 16

#if defined(__gnu_linux__)
#include <execinfo.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

static void stacktrace()
{
#if defined(__gnu_linux__)
  void *buf[MAX_TRACED_STACK_FRAMES];   /* stack frame buffer */
  int nptrs;                            /* number of ptrs */
  char **traces;                        /* string ptrs */
  int i;
  
  nptrs = backtrace(buf, MAX_TRACED_STACK_FRAMES);
  traces = backtrace_symbols(buf, nptrs);
  if (!traces) {
    perror("backtrace_symbols@print_stack_trace");
    exit(EXIT_FAILURE);
  }
  
  for (i = 0; i < nptrs; i++)
    fprintf(stderr, "\t%s\n", traces[i]);
  
  free(traces);
#else
#error
#endif
}

void mv_stacktrace(unsigned long p)
{
#ifndef NDEBUG
  stacktrace();
#endif  
}
