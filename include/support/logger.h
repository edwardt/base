/**
 * @file logger.h
 * 
 * @brief Interface to the logger.
 */
#ifndef MV_LOGGER_H
#define MV_LOGGER_H


typedef enum {
  MV_VERBOSE_NONE = 0,
  MV_VERBOSE_MINIMAL,
  MV_VERBOSE_ALL
} mv_verbose_t;


/**
 * Set the verbosity.
 */
extern void mv_set_verbosity(mv_verbose_t v);

/**
 * Emit the log message.
 */
extern int mv_log(const char *format, ...);

#endif /* MV_LOGGER_H */
