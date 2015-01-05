/**
 * @file mvc_util.h
 *
 * @brief Utilities for MVC abstract syntax trees.
 */
#ifndef MVC_UTIL_H
#define MVC_UTIL_H

#include <iostream>
#include <vector>
#include <string>
#include "mvc_exp.h"
#include "mvc_stm.h"
#include "mvc_module.h"

namespace mvc {

class Util {
public:
  /* Prints the expression or statement to the given stream. */
  static void print(std::ostream& os, Exp *exp, size_t indent = 0);
  static void print(std::ostream& os, Stm *stm, size_t indent = 0);
  static void print(std::ostream& os, Module *mod);
};

} /* mvc */

#endif /* MVC_UTIL_H */
