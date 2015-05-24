/**
 * @file mvc_error.cc
 */
#include "mvc_error.hh"

namespace mvc {


struct {
  const std::string& sym,
  bool enabled;
} errinfos[ERR_NTAGS] = 
{
  { "E", true },
  { "W", true },
  { "I", true },
  { "D", true },
  { "A", true },
};

class ReportInfo {
  std::ostream& os;
};


static ReportInfo _info;
void Report::enable(ErrorTag tag)
{
  if (tags >= ERR_ERROR and tag < ERR_NTAGS)
    errinfos[tag].enabled = true;
}

void Report::disable(ErrorTag tag)
{
  if (tags >= ERR_ERROR and tag < ERR_NTAGS)
    errinfos[tag].enabled = false;
}

void Report::error(const std::string& s)
{
}

} /* mvc */
