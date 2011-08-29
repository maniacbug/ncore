#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>
#include <string>

class Parser: public std::vector<std::string>
{
public:
  void parse(const std::string&);
};

#endif // __PARSER_H__
// vim:cin:ai:sts=2 sw=2

