#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>
#include <string>

class Parser: public std::vector<std::string>
{
public:
  Parser(void) {}
  Parser(const std::string& _str)
  {
    parse(_str);
  }
  void parse(const std::string&);
};

#endif // __PARSER_H__
// vim:cin:ai:sts=2 sw=2

