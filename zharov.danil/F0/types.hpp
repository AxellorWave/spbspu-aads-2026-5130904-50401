#ifndef TYPES_HPP
#define TYPES_HPP
#include <string>
#include <rh_hash_table.hpp>

namespace zharov
{
  struct MenuItem
  {
    int price;
    int prep_time;
    std::string description;
    bool available;
  };

  using ItemTable =
    RHHashTable< std::string, int, std::hash< std::string >, std::equal_to< std::string > >;

  struct Order
  {
    int id;
    ItemTable items;
  };

  struct HistoryEntry
  {
    std::string queue_name;
    int order_id;
    std::string status;
    int profit;
    ItemTable items;
  };

  using MenuTable =
    RHHashTable< std::string, MenuItem, std::hash< std::string >, std::equal_to< std::string > >;
  using QueueTable = RHHashTable< int, Order, std::hash< int >, std::equal_to< int > >;
}
#endif
