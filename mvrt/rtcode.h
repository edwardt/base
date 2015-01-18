/**
 * @file rtcode.h
 *
 * @brief Interface to code, which represents the body of a single function
 * or a reactor.
 */
#ifndef MVRT_CODE_H
#define MVRT_CODE_H


#include "rtoper.h"    /* mvrt_instr_t */

#define MAX_CODE_SIZE 1024
typedef struct mvrt_code {
  int size;
  mvrt_instr_t instrs[MAX_CODE_SIZE];
} mvrt_code_t;


/* Create an empty code struct. */
extern mvrt_code_t *mvrt_code_new();

/* Load a single definition of code from a file. The file position of fp 
   should be be set at the line "{", which starts the body of a function
   or a reactor */
extern mvrt_code_t *mvrt_code_load_file(FILE *fp);

extern int mvrt_code_delete(mvrt_code_t *code);

extern char *mvrt_code_save_str(mvrt_code_t *code);

extern void mvrt_code_print(mvrt_code_t *code);

#endif /* MVRT_CODE_H */
