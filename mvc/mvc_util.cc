/**
 * @file mvc_util.cc
 */
#include <cstdio>
#include <cstdarg>
#include <iostream>
#include <memory>
#include "mvc_util.hh"

namespace mvc {

const static std::size_t _bufsz = 1024;
static std::string _sformat(const char *fmt, va_list ap)
{
  std::unique_ptr<char []> buf(new char[_bufsz]);

  va_list aq;
  va_copy(aq, ap);
  std::size_t n = vsnprintf(&buf[0], _bufsz, fmt, aq);
  va_end(aq);

  if (n > -1  && n < _bufsz)
    return std::string(buf.get());

  buf.reset(new char[n + 1]);
  vsnprintf(&buf[0], n + 1, fmt, ap);

  return std::string(buf.get());
}

std::string Util::sformat(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  std::string s = _sformat(fmt, ap);
  va_end(ap);
  
  return s;
}

} /* mvc */
