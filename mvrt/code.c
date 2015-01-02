/**
 * @file code.c
 *
 * @author cjeong
 */
#include <stdio.h>      /* fprintf */
#include <stdlib.h>     /* malloc */
#include <assert.h>     /* assert */
#include "code.h"


mvrt_code_t *mvrt_code_new()
{
  mvrt_code_t *code = malloc(sizeof(mvrt_code_t));
  code->size = 0;

  return code;
}

int mvrt_code_delete(mvrt_code_t *code)
{
  free(code);
  return 0;
}


void mvrt_code_print(mvrt_code_t *code)
{
  mvrt_instr_t *instr = &code->instrs[0];
  int count = 0;
  while (count < code->size) {
    fprintf(stdout, "CODE[%d]: %s\n", count, mvrt_opcode_str(instr->opcode));
    instr++;
    count++;
  }
}
