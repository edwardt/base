/**
 * @file mvc_base.h
 *
 * @brief Interface to services related to MVC Compiler.
 */
#ifndef MVC_BASE_H
#define MVC_BASE_H

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

#endif /* MVC_BASE_H */
