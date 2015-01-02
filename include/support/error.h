/**
 * @file error.h
 * 
 * @brief Interface to the error reporting module.
 */
#ifndef MV_ERROR_H
#define MV_ERROR_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#if defined(__gnu_linux__)
#define MV_ASSERT(P) \
  mv_stacktrace( (unsigned long) (P) )
#else
#include <cassert>
#define MV_ASSERT(P) assert( (P) )
#endif /* defined(__gnu_linux__) */

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus */

/* Error-emitting functions. */
#define MV_ERROR_ABORT(code, text)                      \
  do {                                                  \
    fprintf(stderr, "%s at \"%s\":%d: %s\n",            \
            text, __FILE__, __LINE__, strerror(code));  \
    abort();                                            \
  } while (0)

#define MV_ERROR(text)                                                  \
  do {                                                                  \
    fprintf(stderr, "%s at \"%s\":%d: %s\n", text, __FILE__, __LINE__); \
  } while (0)


/* Dump stacktrace. */
void mv_stacktrace(unsigned long p);

/* Initialize the signal handler. */
void mv_init_signal_handler();


#ifdef __cplusplus
}
#endif /*__cplusplus */

#endif /* MV_ERROR_H */
