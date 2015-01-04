/**
 * @file repl.c
 *
 * @brief Read-eval-print loop.
 */
#include <stdio.h>     /* fprintf */
#include <string.h>    /* strchr */
#include "command.h"   /* mvsh_command_process */
#include "repl.h"      /* mvsh_repl */

#define MAX_LINE 4096

int mvsh_repl(mv_mqueue_t *mq)
{
  char line[MAX_LINE];
  char *charp;
  while (1) {
    fprintf(stdout, "%s ", MVSH_PROMPT);
    

    if (fgets(line, MAX_LINE, stdin)) {
      if ((charp = strchr(line, '\n')) != NULL)
        *charp = '\0';

      mvsh_command_process(line, mq);
    }
  }

  return 0;
}
