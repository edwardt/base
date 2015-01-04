/**
 * @file repl.h
 *
 * @brief Real-eval-print loop.
 */
#ifndef MVSH_REPL_H
#define MVSH_REPL_H

#include <mq/mqueue.h>    /* mv_mqueue_t */

#define MVSH_PROMPT  "#"

extern int mvsh_repl(mv_mqueue_t *mq);

#endif /* MVSH_REPL_H */
