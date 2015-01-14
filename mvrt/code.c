/**
 * @file code.c
 *
 * @author cjeong
 */
#include <stdio.h>      /* fprintf */
#include <stdlib.h>     /* malloc */
#include <string.h>     /* strchr */
#include <assert.h>     /* assert */
#include "code.h"


static mvrt_code_t *_rtcode_parse(FILE *fp);
static int _rtcode_token_tag(const char *token);
static int _rtcode_parse_nargs(int op);

enum {
  _TOKEN_INVALID = 0,
  _TOKEN_LPAREN  = 2,
  _TOKEN_RPAREN  = 3,
  _TOKEN_ID      = 4,
  _TOKEN_COMMENT = 5,
  _TOKEN_NTAGS   = 6
};

enum {
  _STATE_EXPECT_LPAREN         = 2,
  _STATE_EXPECT_OPER_OR_RPAREN = 3,
  _STATE_EXPECT_ARG            = 4,
  _STATE_DONE                  = 5,
  _STATE_NTAGS                 = 6
};

int _rtcode_token_tag(const char *token)
{
  int len = strlen(token);
  if (len == 1) {
    if (token[0] == '#')
      return _TOKEN_COMMENT;
    else if (token[0] == '{') 
      return _TOKEN_LPAREN;
    else if (token[0] == '}')
      return _TOKEN_RPAREN;
  }
  else if (len > 0) {
    if (token[0] == '#')
      return _TOKEN_COMMENT;
    else
      return _TOKEN_ID;
  }

  return _TOKEN_INVALID;
}

int _rtcode_parse_nargs(int op)
{
  switch (op) {
  case MVRT_OP_PUSHS:
  case MVRT_OP_PUSHI:
  case MVRT_OP_BEQ:
    return 1;
  default:
    break;
  }

  return 0;
}

#define MAX_CODE_LINE 1024
mvrt_code_t *_rtcode_parse(FILE *fp)
{
  /* {
       pushi 1
       pushi 2
       add
     } 
  */
  char line[MAX_CODE_LINE];
  char *token;
  char *charp;
  int toktag;
  int optag;
  int nopers = 0;
  int state = _STATE_EXPECT_LPAREN;
  mvrt_code_t *code = NULL;

  while (state != _STATE_DONE && fgets(line, MAX_CODE_LINE, fp)) {
    if ((charp = strchr(line, '\n')) != NULL)
      *charp = '\0';

    if (strlen(line) == MAX_CODE_LINE) {
      fprintf(stderr, "Line too long --  Reduce to < %d.\n", MAX_CODE_LINE);
      exit(1);
    }

    if (line[0] == '#')
      continue;
    
    if ((token = strtok(line, " \t")) == NULL)
      continue;

    while (token) {
      toktag = _rtcode_token_tag(token);

      if (toktag == _TOKEN_COMMENT)
        break;
      
      switch (state) {
      case _STATE_EXPECT_LPAREN:
        if (toktag == _TOKEN_LPAREN) {
          code = mvrt_code_new();
          state = _STATE_EXPECT_OPER_OR_RPAREN;
        }
        else {
          fprintf(stderr, "S%d: Parse error at token \"%s\".\n", state, token);
          exit(1);
        }
        break;
      case _STATE_EXPECT_ARG:
        switch (optag) {
        case MVRT_OP_PUSHS:
          {
            char *arg = strdup(token+1);
            arg[strlen(arg)-1] = '\0';
            fprintf(stdout, "\treactor[%d]: %s \"%s\"\n", nopers,  
                    mvrt_opcode_str(code->instrs[nopers].opcode), arg);
            code->instrs[nopers].ptr = (mv_ptr_t) arg;
            nopers++;
          }
          break;
        case MVRT_OP_PUSHI:
        case MVRT_OP_BEQ:
          {
            int arg = atoi(token);
            fprintf(stdout, "\treactor[%d]: %s %d\n", nopers,  
                    mvrt_opcode_str(code->instrs[nopers].opcode), arg);
            code->instrs[nopers].ptr = (mv_ptr_t) arg;
            nopers++;
          }
          break;
        default:
          break;
        }
        state = _STATE_EXPECT_OPER_OR_RPAREN;
        break;
      case _STATE_EXPECT_OPER_OR_RPAREN:
        if (toktag == _TOKEN_ID) {
          if ((optag = mvrt_opcode_tag(token)) == -1) {
            fprintf(stderr, "Invalid operator -- \"%s\"", token);
            exit(1);
          }
          else
            code->instrs[nopers].opcode = optag;

          if (_rtcode_parse_nargs(optag) > 0) {
            state = _STATE_EXPECT_ARG;
          }
          else {
            state = _STATE_EXPECT_OPER_OR_RPAREN;
            fprintf(stdout, "\treactor[%d]: %s\n", nopers,  
                    mvrt_opcode_str(code->instrs[nopers].opcode));
            nopers++;
          }
        }
        else if (toktag == _TOKEN_RPAREN) {
          code->size = nopers;
          state = _STATE_DONE;
        }
        else {
          fprintf(stderr, "S%d: Parse error at token \"%s\".\n", state, token);
          exit(1);
        }
        break;
      default:
        assert(0);
        break;
      }
      token = strtok(NULL, " \t");
    }
  }

  return code;
}

mvrt_code_t *mvrt_code_new()
{
  mvrt_code_t *code = malloc(sizeof(mvrt_code_t));
  code->size = 0;

  return code;
}

mvrt_code_t *mvrt_code_load_file(FILE *fp)
{
  return _rtcode_parse(fp);
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
