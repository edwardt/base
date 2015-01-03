/**
 * @file code.h
 *
 * @brief Interface to code.
 */
#ifndef MV_CODE_H
#define MV_CODE_H

#include "operator.h"    /* mvrt_instr_t */

#define MAX_CODE_SIZE 1024
typedef struct mvrt_code {
  int size;
  mvrt_instr_t instrs[MAX_CODE_SIZE];
} mvrt_code_t;


extern mvrt_code_t *mvrt_code_new();
extern int mvrt_code_delete(mvrt_code_t *code);

extern void mvrt_code_print(mvrt_code_t *code);

#endif /* MV_CONTEXT_H */
