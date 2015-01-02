/**
 * @file config.h
 *
 * @brief Interface to configuration of runtime.
 */
#ifndef MVRT_CONFIG_H
#define MVRT_CONFIG_H

#define DEFAULT_CONFIG_FILE ".mvrtrc"

extern int mvrt_config_init();

extern const char *mvrt_config_initfile();

#endif /* MVRT_CONFIG_H */
