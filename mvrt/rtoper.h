/**
 * @file rtoper.h
 *
 * @brief Interface to operators.
 */
#ifndef MVRT_OPER_H
#define MVRT_OPER_H

#include <mv/defs.h>      /* mv_ptr_t */

typedef enum {
  /* nop */
  MVRT_OP_NOP = 0,

  /* sleep */
  MVRT_OP_SLEEP,

  /* arithmetic */
  MVRT_OP_ADD,
  MVRT_OP_SUB,
  MVRT_OP_MUL,
  MVRT_OP_DIV,

  /* logical */
  MVRT_OP_NOT,
  MVRT_OP_AND,
  MVRT_OP_OR,

  /* comparison, predicates */
  MVRT_OP_EQ,
  MVRT_OP_NE,
  MVRT_OP_GT,
  MVRT_OP_GE,
  MVRT_OP_LT,
  MVRT_OP_LE,

  MVRT_OP_INTEGER_P,
  MVRT_OP_FLOAT_P,
  MVRT_OP_STRING_P,
  MVRT_OP_MAP_P,
  MVRT_OP_FUNCTION_P,
  MVRT_OP_EVENT_P,

  /* flow control */
  MVRT_OP_JMP,
  MVRT_OP_BEQ,
  MVRT_OP_BLT,
  MVRT_OP_BLE,
  MVRT_OP_BGT,
  MVRT_OP_BGE,

  MVRT_OP_RET,

  /* value operators */
  MVRT_OP_PAIR_NEW,
  MVRT_OP_PAIR_FIRST,
  MVRT_OP_PAIR_SECOND,
  MVRT_OP_MAP_NEW,
  MVRT_OP_MAP_ADD,
  MVRT_OP_CONS_NEW,
  MVRT_OP_CONS_CAR,
  MVRT_OP_CONS_CDR,
  MVRT_OP_CONS_SETCAR,
  MVRT_OP_CONS_SETCDR,

  /* load/save values from [constant|stack|prop] to [stack|prop] */
  MVRT_OP_GETARG,           /* put the reactor/function arg to stack top */
 
  MVRT_OP_GETF,             /* get field of a record value */
  MVRT_OP_SETF,             /* set field of a record value */
 
  MVRT_OP_PUSHN,            /* push null value */
  MVRT_OP_PUSH0,            /* push integer constant 0 */
  MVRT_OP_PUSH1,            /* push integer constant 1 */
  MVRT_OP_PUSHI,            /* push integer constant */
  MVRT_OP_PUSHS,            /* push string */

  MVRT_OP_POP,

  MVRT_OP_SAVE0,            /* save to temporary 0 */
  MVRT_OP_SAVE1,            /* save to temporary 1 */
  MVRT_OP_LOAD0,            /* load to stack top from temporary 0 */
  MVRT_OP_LOAD1,            /* load to stack top from temporary 1 */

  /* event service */
  MVRT_OP_EVENT_OCCUR,

  /* property service */
  MVRT_OP_PROP_GET,
  MVRT_OP_PROP_SET,
  
  /* funtion service */
  MVRT_OP_CALL_FUNC,        /* call function */
  MVRT_OP_CALL_FUNC_RET,    /* call function with return value */

  MVRT_OP_CALL_RETURN,      /* return the value to caller */
  MVRT_OP_CALL_CONTINUE,    /* resume suspended computation */

  MVRT_OP_NTAGS
} mvrt_opcode_t;

typedef struct mvrt_instr {
  unsigned int opcode  : 8;    /* OP code */
  unsigned int operand : 24;   /* operand, if any */
  mv_ptr_t     ptr;            /* opaque pointer to some value */
} mvrt_instr_t;

/* Returns the string for the given operator. */
extern const char *mvrt_opcode_str(mvrt_opcode_t op);
extern size_t mvrt_opcode_nargs(mvrt_opcode_t op);

/* Returns the opcode tag for the given string. If it's not a valid
   opcode, returns -1.  */
extern int mvrt_opcode_tag(const char *str);

#endif /* MVRT_OPER_H */
