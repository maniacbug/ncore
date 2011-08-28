#include <LineParser.h>

using std::string;
using std::vector;

void LineParser::parse(const string& _in)
{
  if ( _in.size() )
    push_back(_in);
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
