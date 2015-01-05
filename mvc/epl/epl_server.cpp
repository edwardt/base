/**
 * @file epl_server.cpp
 * @brief Implementation of mv::epl::Server class which serves as the
 * main interface to the EPL compiler.
 *
 * @author cjeong
 */
#include "mv/common.h"
#include "mv/epl.h"
#include "mv/epl/epl_parser.h"
#include "mv/epl/epl_analyzer.h"
#include "mv/epl/epl_util.h"

namespace mv { namespace epl {

/*
 * Server
 */
int Server::compile(SourceTag tag, const std::string& str)
{
  /* parse the input code into AST */
  Module *module = Parser::parse(tag, str);
  if (!module) {
    return -1;
  }
  Util::print(std::cout, module);

  /* perform semantic analysis, generate IR, and fill up symbol table */
  SymbolTable *symtab = SymbolTable::getInstance();
  if (Analyzer::analyze(module, symtab) == -1) { 
    return -1;
  }

  /*
  Code *code = EplCodeGenerator::generateCode(module);
  if (!code) {
    return -1;
  }
  */

  return 0;
}


}} /* mv::epl */
