#ifndef MENU_CMD_HPP
#define MENU_CMD_HPP
#include <iostream>
#include "cafe.hpp"

namespace zharov
{
  void menuAdd(std::ostream&, std::istream&, CafeSystem&);
  void menuRemove(std::ostream&, std::istream&, CafeSystem&);
  void menuPrice(std::ostream&, std::istream&, CafeSystem&);
  void menuTime(std::ostream&, std::istream&, CafeSystem&);
  void menuDesc(std::ostream&, std::istream&, CafeSystem&);
  void menuAvailable(std::ostream&, std::istream&, CafeSystem&);
  void menuImport(std::ostream&, std::istream&, CafeSystem&);
  void menuCopy(std::ostream&, std::istream&, CafeSystem&);

  void menuShow(std::ostream&, std::istream&, const CafeSystem&);
  void menuExport(std::ostream&, std::istream&, const CafeSystem&);
  void menuList(std::ostream&, std::istream&, const CafeSystem&);
}
#endif
