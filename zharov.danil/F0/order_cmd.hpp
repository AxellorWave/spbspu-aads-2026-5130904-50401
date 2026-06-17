#ifndef ORDER_CMD_HPP
#define ORDER_CMD_HPP
#include <iostream>
#include "cafe.hpp"

namespace zharov
{
  void orderCreate(std::ostream&, std::istream&, CafeSystem&);
  void orderAdd(std::ostream&, std::istream&, CafeSystem&);
  void orderRemove(std::ostream&, std::istream&, CafeSystem&);
  void orderCancel(std::ostream&, std::istream&, CafeSystem&);

  void orderShow(std::ostream&, std::istream&, const CafeSystem&);
}
#endif
