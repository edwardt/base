/**
 * @file mvc_main.cpp
 *
 * @brief The main for MVC.
 */
#include <cstdio>           /* fprintf */
#include <cstdlib>          /* exit */
#include "mvc_base.h"
#include "mvc_exp.h"
#include "mvc_stm.h"
#include "mvc_parser.h"
#include "mvc_util.h"

using namespace mvc;

/* 
 * the main entry point
 */
int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stdout, "Usage: %s [mvc-file]\n", argv[0]);
    exit(EXIT_SUCCESS);
  }

  Module *mod = Parser::parse(SCT_FILE, std::string(argv[1]));
  Util::print(std::cout, mod);

  return 0;
}
