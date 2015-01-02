/**
 * @file env.h
 *
 * @brief Interface to the VM environment, which maintains a mapping from
 *        names to values.
 */
#ifndef MV_ENV_H
#define MV_ENV_H

#include "name.h"
#include "value.h"


typedef void mv_env_t;


/**
 * Create a new environment.
 */
mv_env_t *mv_env_new(mv_env_t *parent);

/**
 * Find a value for the given name in the given environment.
 */
mv_value_t *mv_env_lookup(mv_env_t *env, mv_name_t *name);

/**
 * Add a (name, value)-pair to the given environment. Returns 0 on success
 * and -1 on failure.
 */
int mv_env_add(mv_env_t *env, mv_name_t *name, mv_value_t *value);

/**
 * Updates the value associated with the given name. Returns 0 on success
 * and -1 on failure.
 */
int mv_env_update(mv_env_t *env, mv_name_t *name, mv_value_t *value);



#endif /* MV_ENV_H */
