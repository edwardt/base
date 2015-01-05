/**
 * @file mvc_parser.cpp
 *
 * @brief Interface to MVC parser.
 *
 * @author cjeong
 */
#include "mvc_parser_driver.h" 
#include "mvc_parser.h" 

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
