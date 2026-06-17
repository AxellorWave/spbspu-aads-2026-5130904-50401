#ifndef CAFE_HPP
#define CAFE_HPP
#include <string>
#include "types.hpp"
#include <rh_hash_table.hpp>

namespace zharov
{
  using Menus =
    RHHashTable< std::string, MenuTable, std::hash< std::string >, std::equal_to< std::string > >;
  using Queues =
    RHHashTable< std::string, QueueTable, std::hash< std::string >, std::equal_to< std::string > >;

  struct CafeSystem
  {
    Menus menus;
    Queues queues;
    std::string history_file;
  };
}
#endif
