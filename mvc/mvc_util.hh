/**
 * @file mvc_util.hh
 *
 * @brief Utilities for MVC abstract syntax trees.
 */
#ifndef MVC_UTIL_HH
#define MVC_UTIL_HH

#include <iostream>
#include <vector>
#include <string>
#include "mvc_exp.hh"
#include "mvc_stm.hh"
#include "mvc_module.hh"

namespace mvc {

class Util {
public:
  /* Prints the expression or statement to the given stream. */
  static void print(std::ostream& os, Exp *exp, size_t indent = 0);
  static void print(std::ostream& os, Stm *stm, size_t indent = 0);
  static void print(std::ostream& os, Module *mod);

  /* Returns a formatted string like sprintf does. */
  static std::string sformat(const std::string& fmt, ...);
  
};

} /* mvc */

#endif /* MVC_UTIL_HH */
