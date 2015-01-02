/**
 * @file operator.c
 */
#include <stdlib.h>      /* malloc */
#include "operator.h"


struct {
  const char *str;    /* string */
  int nargs;          /* number of args on stack */
} _oper_info[] = {
  /* nop */
  { "nop", 0 },

  /* sleep */
  { "sleep", 1 },

  /* arithmetic */
  { "add", 2 },
  { "sub", 2 },
  { "mul", 2 }, 
  { "div", 2 },

  /* logical */
  { "not", 1 },
  { "and", 2 },
  { "or",  2 },

  /* comparison, predicates */
  { "eq", 2 },
  { "ne", 2 },
  { "gt", 2 },
  { "ge", 2 },
  { "lt", 2 },
  { "le", 2 },
  { "integer_p",  1 },
  { "float_p",    1 },
  { "string_p",   1 },
  { "map_p",      1 },
  { "function_p", 1 },
  { "event_p",    1 },

  /* flow control */
  { "jmp", 1 },
  { "beq", 2 },       
  { "blt", 2 },
  { "ble", 2 },
  { "bgt", 2 },
  { "bge", 2 },
  { "ret", 1 },

  /* value builders */
  { "pair_new", 2 },
  { "pair_1st", 1 },
  { "pair_2nd", 1 },
  { "map_new",  0 },
  { "map_add",  3 },
  { "cons",     1 },
  { "car",      2 },
  { "cdr",      2 },
  { "setcar",   2 },
  { "setcdr",   2 },

  /* load/save values */
  { "getarg",   0 },
  { "getp",     1 },
  { "setp",     1 },

  { "getf",     1 },
  { "setf",     1 },

  { "push0",    0 },
  { "push1",    0 },
  { "pushi",    1 },
  { "pushs",    1 },

  { "pop",      0 },

  { "save0",    1 },
  { "save1",    1 },
  { "load0",    1 },
  { "load1",    1 },

  /* events */
  { "event_occur", 2 },

  /* properties */
  { "prop_get",    1 },
  { "prop_set",    2 },

  /* functions */
  { "call_local",      2 },
  { "call_native",     2 },
  { "call_remote",     2 },
  { "call_remote_ret", 2 },
  { "call_return",     2 },
  { "call_continue",   2 },

  { "",            0 }
};

const char *mvrt_opcode_str(mvrt_opcode_t op)
{
  return _oper_info[op].str;
}

size_t mvrt_opcode_nargs(mvrt_opcode_t op)
{
  return _oper_info[op].nargs;
}

int mvrt_opcode_tag(const char *str)
{
  /* TODO: make it efficient - search tree or trie */

  int op;
  for (op = 0; op < MVRT_OP_NTAGS; op++) {
    if (!strcmp(_oper_info[op].str, str))
      return op;
  }
  return -1;
}
