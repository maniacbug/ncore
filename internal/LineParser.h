#ifndef __LINEPARSER_H__
#define __LINEPARSER_H__

#include <vector>
#include <string>

class LineParser: public std::vector<std::string>
{
public:
  void parse(const std::string&);
};

#endif // __LINEPARSER_H__
// vim:cin:ai:sts=2 sw=2

