/**
 * @file mvc_main.cc
 *
 * @brief The main for MVC.
 */
#include <cstdio>           /* fprintf */
#include <cstdlib>          /* exit */
#include "mvc_base.hh"
#include "mvc_exp.hh"
#include "mvc_stm.hh"
#include "mvc_parser.hh"
#include "mvc_util.hh"

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
