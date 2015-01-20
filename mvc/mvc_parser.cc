/**
 * @file mvc_parser.cc
 */
#include "mvc_parser_driver.hh" 
#include "mvc_parser.hh" 

namespace mvc {

/*
 * Parser
 */
Module *Parser::parse(SourceTag tag, const std::string& str)
{
  ParserDriver driver;
  Module *module = driver.parse(tag, str);

  return module;
}

} /* mvc */
