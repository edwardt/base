/**
 * @file eval.c
 *
 * @author cjeong
 */
#include <stdio.h>       /* fprintf */
#include <stdlib.h>      /* malloc */
#include <string.h>      /* strstr */
#include <pthread.h>     /* pthread_create */
#include <dlfcn.h>       /* dlopen */
#include <assert.h>      /* assert */
#include <mq/mqueue.h>   /* mv_mqueue_t */
#include <mv/device.h>   /* mv_device_t */
#include "rtprop.h"      /* mvrt_prop_t */
#include "rtvalue.h"     /* mvrt_value_t */
#include "rtfunc.h"      /* mvrt_func_t */
#include "message.h"     /* mvrt_message_t */
#include "context.h"     /* mvrt_stack_t */
#include "eval.h"


static int _eval(mvrt_code_t *code, mvrt_context_t *ctx);
static int _eval_instr(mvrt_instr_t *instr, mvrt_context_t *ctx);
static int _eval_branch(mvrt_instr_t *instr, mvrt_context_t *ctx);
static int _eval_push(mvrt_instr_t *instr, mvrt_context_t *ctx);
static int _eval_cons(mvrt_instr_t *instr, mvrt_context_t *ctx);
static int _eval_stackop(mvrt_instr_t *instr, mvrt_context_t *ctx);
static int _eval_call_func(mvrt_instr_t *instr, mvrt_context_t *ctx);
static int _eval_call_native(mvrt_value_t f, mvrt_value_t a, mvrt_context_t *c);
static int _eval_call_return(mvrt_instr_t *instr, mvrt_context_t *ctx);
static int _eval_call_continue(mvrt_instr_t *instr, mvrt_context_t *ctx);
static mvrt_func_t _eval_get_func(const char *func_s);

extern mv_mqueue_t *mq;
extern char *dest;

typedef enum {
  _EVAL_FAILURE = -1,
  _EVAL_SUSPEND = -2,
  _EVAL_RETURN  = -3
}  _eval_ret_t;

int _eval(mvrt_code_t *code, mvrt_context_t* ctx)
{
  mvrt_instr_t *instr = code->instrs + ctx->iptr;
  mvrt_continue_t *cont = NULL;
  int next_ip = 0;
  while (instr && ctx->iptr < code->size) {
    const char *opstr = mvrt_opcode_str(instr->opcode);
    fprintf(stdout, "\tEVAL[%d]: %ss\n", ctx->iptr, opstr);
    next_ip = _eval_instr(instr, ctx);
    if (next_ip == _EVAL_FAILURE) {
      fprintf(stderr, "Evaluation error at %s.\n", opstr);
      break;
    }
    else if (next_ip == _EVAL_SUSPEND) {
      mvrt_continue_t *cont = mvrt_continuation_new(code, ctx);
      break;
    }
    else if (next_ip == _EVAL_RETURN) {
      break;
    }
    ctx->iptr = next_ip;
    instr = code->instrs + ctx->iptr;
  }

  return next_ip;
}

/* Returns the next IP >= 0 on normal situation. Returns -1 on error.
   Returns -2 when we need to suspend execution and create a continuation 
   point, to be rescheduled on some future event. */
int _eval_instr(mvrt_instr_t *instr, mvrt_context_t *ctx)
{
  mvrt_stack_t *stack = ctx->stack;
  int ip = ctx->iptr;

  int retval;
  switch (instr->opcode) {
  /* flow control */
  case MVRT_OP_JMP:
  case MVRT_OP_BEQ:
  case MVRT_OP_RET:
    return _eval_branch(instr, ctx);
  case MVRT_OP_PUSHI:
  case MVRT_OP_PUSHS:
    return _eval_push(instr, ctx);
  case MVRT_OP_POP:
    mvrt_stack_pop(stack);
    return ip + 1;

  /* value builder */
  case MVRT_OP_CONS_NEW:
  case MVRT_OP_CONS_CAR:
  case MVRT_OP_CONS_CDR:
  case MVRT_OP_CONS_SETCAR:
  case MVRT_OP_CONS_SETCDR:
    return _eval_cons(instr, ctx);

  /* load/save to/from stack */
  case MVRT_OP_GETARG:
  case MVRT_OP_GETF:
  case MVRT_OP_SETF:
    return _eval_stackop(instr, ctx);

  /* properties */
  case MVRT_OP_PROP_GET:
    return _eval_prop_get(instr, ctx);
  case MVRT_OP_PROP_SET:
    return _eval_prop_set(instr, ctx);

  /* function calls */
  case MVRT_OP_CALL_FUNC:
  case MVRT_OP_CALL_FUNC_RET:
    return _eval_call_func(instr, ctx);
  case MVRT_OP_CALL_RETURN:
    return _eval_call_return(instr, ctx);
  case MVRT_OP_CALL_CONTINUE:
    return _eval_call_continue(instr, ctx);
  default:
    break;
  }

  assert(0 && "Either OP not implemented or something wrong.");
  return ip + 1;
}

int _eval_branch(mvrt_instr_t *instr, mvrt_context_t *ctx)
{
  mvrt_stack_t *stack = ctx->stack;
  int ip = ctx->iptr;

  /* 
     BEQ <branch ip>

     Pop two scalar values from the stack and compare. If they are equal,
     jump to <branch ip>. Otherwise, just increase the ip by 2.
   */
  int jmpval;
  mvrt_value_t val0;
  mvrt_value_t val1;
  switch (instr->opcode) {
  case MVRT_OP_JMP:
    jmpval = (int) instr->ptr;
    return jmpval;
  case MVRT_OP_BEQ:
    jmpval = (int) instr->ptr;
    val0 = mvrt_stack_pop(stack);
    val1 = mvrt_stack_pop(stack);
    if (mvrt_value_eq(val0, val1))
      return jmpval;
    break;
  case MVRT_OP_RET:
    return _EVAL_RETURN;
  default:
    break;
  }

  return ip + 1;
}

int _eval_push(mvrt_instr_t *instr, mvrt_context_t *ctx)
{
  mvrt_stack_t *stack = ctx->stack;
  int ip = ctx->iptr;

  /*
    PUSH <arg>

    Transforms scalar arg (which is int, char *, etc.) into mvrt_value_t
    and push it into stack.
  */

  switch (instr->opcode) {
  case MVRT_OP_PUSHI:
    {
      int intval = (int) instr->ptr;
      mvrt_value_t intval_v = mvrt_value_int(intval);
      mvrt_stack_push(stack, intval_v);
    }
    break;
  case MVRT_OP_PUSHS:
    {
      char *strval = (char *) instr->ptr;
      mvrt_value_t str_v = mvrt_value_string(strval);
      mvrt_stack_push(stack, str_v);
    }
    break;
  default:
    return _EVAL_FAILURE;
  }
  return ip + 1;
}

int _eval_cons(mvrt_instr_t *instr, mvrt_context_t *ctx)
{
  mvrt_stack_t *stack = ctx->stack;
  int ip = ctx->iptr;
  
  mvrt_value_t val0;
  mvrt_value_t val1;
  mvrt_value_t cons;
  switch (instr->opcode) {
  case MVRT_OP_CONS_NEW:
    cons = mvrt_value_cons();
    val0 = mvrt_stack_pop(stack);
    mvrt_value_cons_setcar(cons, val0);
    mvrt_value_cons_setcdr(cons, mvrt_value_null());
    mvrt_stack_push(stack, cons);
    break;
  case MVRT_OP_CONS_CAR:
    cons = mvrt_stack_pop(stack);
    val0 = mvrt_value_cons_car(cons);
    mvrt_stack_push(stack, val0);
    break;
  case MVRT_OP_CONS_CDR:
    cons = mvrt_stack_pop(stack);
    val0 = mvrt_value_cons_cdr(cons);
    mvrt_stack_push(stack, val0);
    break;
  case MVRT_OP_CONS_SETCAR:
    cons = mvrt_stack_pop(stack);
    val0 = mvrt_stack_pop(stack);
    mvrt_value_cons_setcar(cons, val0);
    mvrt_stack_push(stack, cons);
    break;
  case MVRT_OP_CONS_SETCDR:
    cons = mvrt_stack_pop(stack);
    val0 = mvrt_stack_pop(stack);
    mvrt_value_cons_setcdr(cons, val0);
    mvrt_stack_push(stack, cons);
    break;
  default:
    assert(0 && "Invalid OPCODE.");
    break;
  }

  return ip + 1;
}

int _eval_stackop(mvrt_instr_t *instr, mvrt_context_t *ctx)
{
  mvrt_stack_t *stack = ctx->stack;
  int ip = ctx->iptr;

  mvrt_value_t val0;
  mvrt_value_t val1;
  mvrt_value_t val2;
  mvrt_value_t val3;
  switch (instr->opcode) {
  case MVRT_OP_GETARG:
    val0 = ctx->arg;
    mvrt_stack_push(stack, val0);
    break;
  case MVRT_OP_GETF:
    val0 = mvrt_stack_pop(stack);
    val1 = mvrt_stack_pop(stack);
    val2 = mvrt_value_map_lookup(val0, val1);
    mvrt_stack_push(stack, val2);
    break;
  case MVRT_OP_SETF:
    val0 = mvrt_stack_pop(stack);
    val1 = mvrt_stack_pop(stack);
    val2 = mvrt_stack_pop(stack);
    val3 = mvrt_value_map_add(val0, val1, val2);
    mvrt_stack_push(stack, val3);
    break;
  default:
    assert(0 && "Not implemented yet");
    break;
  }

  return ip + 1;
}

int _eval_prop_get(mvrt_instr_t *instr, mvrt_context_t *ctx)
{
  mvrt_stack_t *stack = ctx->stack;
  int ip = ctx->iptr;

  mvrt_value_t prop_v = mvrt_stack_pop(stack);
  char *prop = mvrt_value_string_get(prop_v);
  mvrt_prop_t mvprop = mvrt_prop_lookup(mv_device_self(), prop);
  assert(mvprop && "No such property exists.");

  mvrt_value_t value_v = mvrt_prop_getvalue(mvprop);
  mvrt_stack_push(stack, value_v);

  return ip + 1;
}

int _eval_prop_set(mvrt_instr_t *instr, mvrt_context_t *ctx)
{
  mvrt_stack_t *stack = ctx->stack;
  int ip = ctx->iptr;

  mvrt_value_t prop_v = mvrt_stack_pop(stack);
  mvrt_value_t value_v = mvrt_stack_pop(stack);
  char *prop = mvrt_value_string_get(prop_v);
  mvrt_prop_t mvprop = mvrt_prop_lookup(mv_device_self(), prop);
  assert(prop && "No such property exists.");
  
  if (mvrt_prop_setvalue(mvprop, value_v) == -1)
    return _EVAL_FAILURE;

  return ip + 1;
}

int _eval_call_local(mvrt_instr_t *instr, mvrt_context_t *ctx)
{
  mvrt_stack_t *stack = ctx->stack;
  int ip = ctx->iptr;

  assert(0 && "Not implemented yet");

  return _EVAL_FAILURE;
}

mvrt_func_t _eval_get_func(const char *func_s)
{
  mvrt_func_t func;
  char *charp;

  if (((charp = strstr(func_s, ":")) == NULL) || charp == func_s) {
    /* "foo" or ":foo" - local function */
    if (charp == func_s)
      func = mvrt_func_lookup(mv_device_self(), func_s);
    else
      func = mvrt_func_lookup(mv_device_self(), func_s+1);
  }
  else {
    /* "dev:foo" - remote function */
    char *name = strdup(func_s + 1);
    char save = *charp;
    *charp = '\0';
    func = mvrt_func_lookup(func_s, name);
    if (func == 0)
      func = mvrt_func_new(func_s, name, MVRT_FUNC_GLOBAL);
    *charp = save;
  }

  return func;
}

int _eval_call_func(mvrt_instr_t *instr, mvrt_context_t *ctx)
{
  mvrt_stack_t *stack = ctx->stack;
  int ip = ctx->iptr;
  
  /*
     CALL_FUNC[_RET]

     Call a remote function with/without waiting for the return value. Stack
     top should contain function value, number of arguments, and arguments.
   */
  mvrt_value_t func_v = mvrt_stack_pop(stack);
  mvrt_value_t farg_v = mvrt_stack_pop(stack);

  char *func_s = (char *) mvrt_value_string_get(func_v);
  mvrt_func_t func = _eval_get_func(func_s);
  if (mvrt_func_tag(func) == MVRT_FUNC_NATIVE)
    return _eval_call_native(func_v, farg_v, ctx);

  const char *dev = mvrt_func_dev(func);
  const char *name = mvrt_func_name(func);
  const char *destaddr = mv_device_addr(dev);
  mvrt_native_t *native = NULL;

  static char msg[4096];
  int rid = 0;

  switch (mvrt_func_tag(func)) {
    native = mvrt_func_native(func);
    if (!native->func1) {
      void *handle = dlopen(native->lib, RTLD_NOW);
      if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
      }
      char *name = native->name;
      native->func1 = (mvrt_native_func1_t) dlsym(handle, name);
      native->func2 = (mvrt_native_func2_t) dlsym(handle, name);
    }

    break;
  case MVRT_FUNC_LOCAL:
    assert(0 && "Not implemented yet.");
    break;
  case MVRT_FUNC_GLOBAL:
    switch (instr->opcode) {
    case MVRT_OP_CALL_FUNC:
      sprintf(msg, 
              "%s {"
              " \"tag\":\"FUNC_CALL\", "
              " \"src\":\"%s\", "
              " \"arg\":"
              "{ \"name\": \"%s\", \"funarg\" : %s } }",
              destaddr, mv_mqueue_addr(mq), name, mvrt_value_to_str(farg_v));
      mv_mqueue_put(mq, msg);
      break;
    case MVRT_OP_CALL_FUNC_RET:
      sprintf(msg, 
              "%s {"
              " \"tag\":\"FUNC_CALL_RET\", "
              " \"src\":\"%s\", "
              " \"arg\":"
              "{ \"name\": \"%s\", \"funarg\" : %s, \"retid\" : %d, "
              "  \"retaddr\": \"%s\" } }",
              destaddr, mv_mqueue_addr(mq), name, mvrt_value_to_str(farg_v), 
              rid, mv_mqueue_addr(mq));
      mv_mqueue_put(mq, msg);
      return _EVAL_SUSPEND;
    case MVRT_FUNC_NATIVE:
      assert(0 && "Native function should not reach here.");
    default:
      break;
    }
  default:
    break;
  }

  return ip + 1;
}

int _eval_call_native(mvrt_value_t func_v, mvrt_value_t farg_v, 
                      mvrt_context_t *ctx)
{
  mvrt_stack_t *stack = ctx->stack;
  int ip = ctx->iptr;

  mvrt_func_t func = mvrt_value_func_get(func_v);
  mvrt_native_t *native = mvrt_func_native(func);

  /* get function */
  if (!native->func1) {
    void *handle = dlopen(native->lib, RTLD_NOW);
    if (!handle) {
      fprintf(stderr, "%s\n", dlerror());
      exit(EXIT_FAILURE);
    }
    char *name = native->name;
    native->func1 = (mvrt_native_func1_t) dlsym(handle, name);
    native->func2 = (mvrt_native_func2_t) dlsym(handle, name);
  }

  /* prepare args */
  int nargs = 0;
  int arg1 = 0;
  int arg2 = 0;
  if (mv_value_tag(farg_v) == MV_VALUE_CONS) {
    /* two integers */
    nargs = 2;
    mv_value_t car = mv_value_cons_car((mv_value_t) farg_v);
    mv_value_t cdr = mv_value_cons_cdr((mv_value_t) farg_v);
    mv_value_t cdar = mv_value_cons_car((mv_value_t) cdr);
    arg2 = mv_value_int_get(car);
    arg1 = mv_value_int_get(cdar);
    fprintf(stdout, "arg1 = %d, arg2 = %d\n", arg1, arg2);
    
  }
  else {
    /* integer */
    nargs = 1;
    arg1 = mv_value_int_get(farg_v);
    fprintf(stdout, "arg1 = %d\n", arg1);
  }

  /* call function */
  int retval = 0;
  if (nargs == 1) {
    retval = (native->func1)(arg1);
  }
  else
    (native->func2)(arg1, arg2);

  mvrt_value_t retval_v = mvrt_value_int(retval);
  mvrt_stack_push(stack, retval_v);

  return ip + 1;
}

int _eval_call_return(mvrt_instr_t *instr, mvrt_context_t *ctx)
{
  mvrt_stack_t *stack = ctx->stack;
  int ip = ctx->iptr;

  mvrt_value_t retid_v = mvrt_stack_pop(stack);
  mvrt_value_t retaddr_v = mvrt_stack_pop(stack);
  mvrt_value_t retval_v = mvrt_stack_pop(stack);

  int retid = mvrt_value_int_get(retid_v);
  const char *retaddr = mvrt_value_string_get(retaddr_v);

  static char msg[4096];
  sprintf(msg, 
          "%s {"
          " \"tag\":\"FUNC_RET\", "
          " \"src\":\"%s\", "
          " \"arg\":"
          "{ \"retid\": \"%d\", \"retval\" : %s } }",
          retaddr, mv_mqueue_addr(mq), retid, mvrt_value_to_str(retval_v));

  return ip + 1;
}

int _eval_call_continue(mvrt_instr_t *instr, mvrt_context_t *ctx)
{
  mvrt_stack_t *stack = ctx->stack;
  int ip = ctx->iptr;

  mvrt_value_t retid_v = mvrt_stack_pop(stack);
  mvrt_value_t retval_v = mvrt_stack_pop(stack);
  int retid = mvrt_value_int_get(retid_v);

  mvrt_continue_t *cont = mvrt_continuation_get(retid);
  mvrt_context_t *cont_ctx = cont->ctx;
  return _eval(cont->code, cont_ctx);
}


/*
 * Functions for the eval interface.
 */
int mvrt_eval_reactor(mvrt_reactor_t *reactor, mvrt_value_t event)
{
  fprintf(stdout, "EVAL: "); 
  mvrt_value_print(event);

  mvrt_event_t evtype = mvrt_value_event_data(event);
  mvrt_value_t evdata = mvrt_value_event_data(event);

  /* Create a new context
     . code
     . iptr = 0
     . stack is newly created with the evdata as its only element
  */
  mvrt_context_t *ctx = mvrt_context_new();
  ctx->iptr = 0;
  ctx->stack = mvrt_stack_new();
  ctx->arg = evdata;
  mvrt_stack_push(ctx->stack, evdata);
  
  int retval = _eval(reactor->code, ctx);
  
  if (retval != _EVAL_SUSPEND)
    mvrt_context_delete(ctx);
  
  return retval;
}
