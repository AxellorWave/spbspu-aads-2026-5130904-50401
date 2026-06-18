#ifndef ANALYZE_HPP
#define ANALYZE_HPP
#include <iostream>
#include "cafe.hpp"

namespace zharov
{
  void analyzeItem(std::ostream&, std::istream&, const CafeSystem&);
  void analyzeQueue(std::ostream&, std::istream&, const CafeSystem&);
}
#endif
