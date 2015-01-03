/**
 * @file repl.c
 *
 * @brief Read-eval-print loop.
 */
#include <stdio.h>     /* fprintf */
#include <string.h>    /* strchr */
#include "repl.h"

#define MAX_LINE 4096

int repl()
{
  char line[MAX_LINE];
  char *charp;
  while (1) {
    fprintf(stdout, "%s ", MVSH_PROMPT);
    

    fgets(line, MAX_LINE, stdin);
    if ((charp = strchr(cmdline, '\n')) != NULL)
      *charp = '\0';

    
  }

  return 0;
}
