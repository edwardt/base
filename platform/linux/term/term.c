/**
 * @file term.c
 *
 * @author Library for terminal I/O.
 */
#include <stdio.h>
#include <mv/value.h>


extern int term_print(mv_value_t v)
{
  mv_value_print(v);
}

/* Read a single line from a console and returns a string value. */
extern mv_value_t term_readline()
{
  static char line[4096];
  fgets(line, 4096, stdin);

  return mv_value_string(&line[0]);
}
