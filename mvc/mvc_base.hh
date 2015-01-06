/**
 * @file mvc_base.hh
 *
 * @brief Interface to services related to MVC Compiler.
 */
#ifndef MVC_BASE_HH
#define MVC_BASE_HH

#include <string>

namespace mvc {


enum SourceTag {
  SCT_NONE,
  SCT_FILE,
  SCT_STDIN,
  SCT_STRING,
  SCT_NTAGS
};
} /* mvc */

#endif /* MVC_BASE_HH */
