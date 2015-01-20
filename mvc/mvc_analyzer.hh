/**
 * @file mvc_analyzer.hh
 *
 * @brief Server class for EPL semantics analysis.
 */
#ifndef MVC_ANALYZER_HH
#define MVC_ANALYZER_HH

#include "mvc_module.hh"
#include "mvc_symtab.hh"
#include "mvc_pass.hh"

namespace mvc {

/**
 * @class Analyzer
 */
class AnalyzerImpl;
class Analyzer : public Pass {
public:
  Analyzer(Module *mod, SymTab *symtab);
  ~Analyzer();

public:
  int run();

private:
  AnalyzerImpl *_impl;
};

} /* mvc */

#endif /* MVC_ANALYZER_HH */
