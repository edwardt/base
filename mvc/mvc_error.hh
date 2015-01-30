/**
 * @file mvc_error.hh
 *
 * @brief Functions for error reporting.
 */
#ifndef MVC_ERROR_HH
#define MVC_ERROR_HH

#include <iostream>
#include <string>

namespace mvc {

#if defined(__gnu_linux__)
#  define MVC_ASSERT(P) \
  Util::stacktrace( (unsigned long) (P) )
#else
#  include <cassert>
#  define MVC_ASSERT(P) assert( (P) )
#endif /* defined(__gnu_linux__) */

typedef enum {
  ERR_ERROR,
  ERR_WARNING,
  ERR_INFO,
  ERR_DEBUG,
  ERR_ASSERT,
  ERR_NTAGS
} ErrorTag;

class Report {

  static int setOutput(std::ostream& os);

  static void enable(ErrorTag tag);
  static void disable(ErrorTag tag);
  
  /* Emits an error/warning message and increment the error/warning count. */
  static void error(const std::string& s);
  static void warn(const std::string& s);
  
  static void getCount(ErrorTag tag);

  /* prints strerror(errcode) */
  static void perror(int errcode, const std::string& s);

};

} /* mvc */

#endif /* MVC_ERROR_HH */
