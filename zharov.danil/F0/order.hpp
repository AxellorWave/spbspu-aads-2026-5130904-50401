#ifndef ORDER_HPP
#define ORDER_HPP
#include <iostream>
#include <string>
#include <rh_hash_table.hpp>

namespace zharov
{
  struct CafeSystem;

  using ItemTable =
    RHHashTable< std::string, int, std::hash< std::string >, std::equal_to< std::string > >;

  struct Order
  {
    int id;
    ItemTable items;
  };

  void orderCreate(std::ostream&, std::istream&, CafeSystem&);
  void orderAdd(std::ostream&, std::istream&, CafeSystem&);
  void orderRemove(std::ostream&, std::istream&, CafeSystem&);
  void orderCancel(std::ostream&, std::istream&, CafeSystem&);

  void orderShow(std::ostream&, std::istream&, const CafeSystem&);
}
#endif
