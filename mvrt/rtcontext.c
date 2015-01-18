/**
 * @file rtcontext.c
 *
 * @author cjeong
 */
#include <stdio.h>       /* fprintf */
#include <stdlib.h>      /* malloc */
#include <assert.h>      /* assert */
#include "rtcontext.h"


static mvrt_stack_t *_rtstack_copy(mvrt_stack_t *stack);

mvrt_stack_t *_rtstack_copy(mvrt_stack_t *stack)
{
  mvrt_stack_t *copy = mvrt_stack_new();
  int i;
  for (i = 0; i < stack->sptr; i++) {
    copy->values[i] = stack->values[i];
  }
  copy->sptr = stack->sptr;

  return copy;
}


/*
 * Functions for context.
 */
mvrt_stack_t *mvrt_stack_new()
{
  mvrt_stack_t *stack = malloc(sizeof(mvrt_stack_t));
  stack->sptr = 0;
  stack->values[0] = mv_value_null();

  return stack;
}

int mvrt_stack_delete(mvrt_stack_t *stack)
{
  free(stack);

  return 0;
}

mv_value_t mvrt_stack_pop(mvrt_stack_t *stack)
{
  assert(stack->sptr > 0);
  mv_value_t top = stack->values[stack->sptr--];

  return top;
}

mv_value_t mvrt_stack_top(mvrt_stack_t *stack)
{
  return stack->values[stack->sptr];
}

int mvrt_stack_push(mvrt_stack_t *stack, mv_value_t value)
{
  assert(stack->sptr < MAX_STACK_SIZE - 1);
  stack->values[++stack->sptr] = value;

  return stack->sptr;
}

mvrt_context_t *mvrt_context_new(mvrt_code_t *code)
{
  mvrt_context_t *ctx = malloc(sizeof(mvrt_context_t));
  ctx->code = code;
  ctx->iptr = 0;
  ctx->stack = NULL;

  return ctx;
}

int mvrt_context_delete(mvrt_context_t *ctx)
{
  if (ctx->stack)
    mvrt_stack_delete(ctx->stack);
  free(ctx);

  return 0;
}


#define MAX_CONT_TABLE 1024
static mvrt_continue_t _cont_table[MAX_CONT_TABLE];
static int _contid = 0;
int mvrt_continuation_new(mvrt_context_t *ctx)
{
  if (_contid == MAX_CONT_TABLE) {
    fprintf(stdout, "MAX contination table reached. Reimplement!\n");
    exit(1);
  }

  mvrt_continue_t *cont = _cont_table + _contid;
  cont->id = _contid++;
  cont->ctx = mvrt_context_new(ctx->code);
  cont->ctx->stack = _rtstack_copy(ctx->stack);
  cont->ctx->iptr = ctx->iptr + 1;

  return cont->id;
}

int mvrt_continuation_delete(mvrt_continue_t *cont)
{
  if (cont->ctx)
    mvrt_context_delete(cont->ctx);
  free(cont);

  return 0;
}

mvrt_continue_t *mvrt_continuation_get(int id)
{
  return _cont_table + id;
}
