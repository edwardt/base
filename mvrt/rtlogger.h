/**
 * @file rtlogger.h
 * 
 * @brief Interface to the logger.
 */
#ifndef MVRT_LOGGER_H
#define MVRT_LOGGER_H


typedef enum {
  MV_VERBOSE_NONE = 0,
  MV_VERBOSE_MINIMAL,
  MV_VERBOSE_ALL
} mv_verbose_t;


/* Set the verbosity. */
extern void mvrt_set_verbosity(mv_verbose_t v);

/* Emit the log message. */
extern int mvrt_log(const char *format, ...);

#endif /* MVRT_LOGGER_H */
