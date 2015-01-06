/**
 * @file svr_message.cc
 */
#include "svr_message.hh"

#include <string>      /* std::string */
#include <iostream>    /* std::cout */
#include <sstream>     /* std::stringstream */

namespace mv { namespace server {

static const std::string _mtags[] = {
  "DEVADD",
  "DEVREM",
  "TAGADD",
  "TAGREM",
  "DADDTG",
  "DREMTG",
  "DEVON",
  "DEVOFF",
  "DEFADR",
  "INVLID"
};

void _split_string(const std::string& str, char delim, 
                   std::vector<std::string> &tokens)
{
  std::stringstream ss(str);
  std::string token;
  while (std::getline(ss, token, delim)) {
    tokens.push_back(token);
  }
}

std::vector<std::string> _split(const std::string& str, char delim) 
{
  std::vector<std::string> tokens;
  _split_string(str, delim, tokens);

  return tokens;
}

Message::Message(const std::string& str) : _tag(MessageNTags)
{
  std::vector<std::string> tokens = _split(str, ' ');
  for (int i = 0; i < MessageNTags; ++i) {
    if (_mtags[i] == tokens[0]) {
      std::cout << _mtags[i] << std::endl;
      _tag = (MessageTag) i;
      break;
    }
  }

  int ntokens = tokens.size();
  for (int i = 1; i < ntokens; ++i) {
    _args.push_back(tokens[i]);
  }
}

Message::~Message()
{
}

const std::string& Message::getArgAt(size_t i)
{
  return _args[i];
}


/*
 * MessageFactory
 */
Message *MessageFactory::build(const std::string& str)
{
  /* TODO: message pool + maintain list of messages for garbage collection, 
     etc. */
  return new Message(str);
}


}} /* mv::message */
