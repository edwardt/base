/**
 * @file context.h
 *
 * @brief Interface to evaluation context objects.
 */
#ifndef MV_CONTEXT_H
#define MV_CONTEXT_H

#include "rtvalue.h"   /* mvrt_value_t */
#include "code.h"      /* mvrt_code_t */

/*
 * Stack
 */
#define MAX_STACK_SIZE 1024
typedef struct mvrt_stack {
  size_t sptr;
  mvrt_value_t values[MAX_STACK_SIZE];
} mvrt_stack_t;

/*
 * Context
 */
typedef struct mvrt_context {
  int iptr;              /* index to code array */
  mvrt_stack_t *stack;   /* stack */
  mvrt_value_t arg;      /* argument to reactor/function */
} mvrt_context_t;


/*
 * Continuation
 */
typedef struct mvrt_continue {
  int id;                /* id of continuation - use as "retid" */
  mvrt_code_t *code;     /* code for which to continue execution */
  mvrt_context_t *ctx;   /* execution context */
} mvrt_continue_t;


extern mvrt_code_t *mvrt_code_new();
extern int mvrt_code_delete(mvrt_code_t *code);

extern mvrt_stack_t *mvrt_stack_new();
extern int mvrt_stack_delete(mvrt_stack_t *stk);
extern mvrt_value_t mvrt_stack_top(mvrt_stack_t *stk);
extern mvrt_value_t mvrt_stack_pop(mvrt_stack_t *stk);
extern int mvrt_stack_push(mvrt_stack_t *stk, mvrt_value_t value);

extern mvrt_context_t *mvrt_context_new();
extern int mvrt_context_delete(mvrt_context_t *ctx);

extern mvrt_continue_t *mvrt_continuation_new(mvrt_code_t *, mvrt_context_t *);
extern int mvrt_contination_delete(mvrt_continue_t *cont);
extern mvrt_continue_t *mvrt_continuation_get(int id);

#endif /* MV_CONTEXT_H */
