/**
 * @file epl_util.h
 * @brief Utilities for EPL abstract syntax trees
 *
 * @author cjeong
 */
#ifndef MV_EPL_UTIL_H
#define MV_EPL_UTIL_H

#include <iostream>
#include <vector>
#include <string>
#include "mv/epl/epl_exp.h"
#include "mv/epl/epl_stm.h"
#include "mv/epl/epl_datatype.h"
#include "mv/epl/epl_module.h"

namespace mv { namespace epl {

class Util {
public:
  /* Prints the expression or statement to the given stream. */
  static void print(std::ostream& os, Exp *exp, size_t indent = 0);
  static void print(std::ostream& os, Stm *stm, size_t indent = 0);
  static void print(std::ostream& os, Datatype *type, size_t indent = 0);
  static void print(std::ostream& os, Module *mod);
};

}} /* mv::epl */

#endif /* MV_EPL_UTIL_H */
