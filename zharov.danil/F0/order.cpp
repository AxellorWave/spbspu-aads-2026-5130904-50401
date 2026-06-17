#include "order.hpp"
#include <iomanip>
#include <stdexcept>
#include <string>
#include "cafe.hpp"
#include "history.hpp"

namespace
{
  bool hasMoreArgs(std::istream& in)
  {
    while (in.peek() == ' ')
    {
      in.get();
    }
    return in.peek() != '\n' && in.peek() != EOF;
  }
}

void zharov::orderCreate(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string queueName;
  in >> queueName;
  if (!cafe.queues.has(queueName))
  {
    throw std::invalid_argument("Queue not found");
  }
  int orderId = 0;
  in >> orderId;
  zharov::QueueTable& queue = cafe.queues.at(queueName);
  if (queue.has(orderId))
  {
    throw std::invalid_argument("Order already exists");
  }
  queue.add(orderId, zharov::Order{orderId, zharov::ItemTable{}});
  out << "<OK: Order #" << orderId << " created in queue \"" << queueName << "\">\n";
}

void zharov::orderAdd(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string queueName;
  in >> queueName;
  if (!cafe.queues.has(queueName))
  {
    throw std::invalid_argument("Queue not found");
  }
  int orderId = 0;
  in >> orderId;
  if (!cafe.queues.at(queueName).has(orderId))
  {
    throw std::invalid_argument("Order not found");
  }
  std::string menuName;
  in >> menuName;
  if (!cafe.menus.has(menuName))
  {
    throw std::invalid_argument("Menu not found");
  }
  std::string itemName;
  in >> itemName;
  if (!cafe.menus.at(menuName).has(itemName))
  {
    throw std::invalid_argument("Item not found");
  }
  if (!cafe.menus.at(menuName).at(itemName).available)
  {
    throw std::invalid_argument("Item unavailable");
  }
  int count = 1;
  if (hasMoreArgs(in))
  {
    in >> count;
  }
  zharov::Order& order = cafe.queues.at(queueName).at(orderId);
  if (order.items.has(itemName))
  {
    order.items.at(itemName) += count;
  }
  else
  {
    order.items.add(itemName, count);
  }
  out << "<OK: Added " << count << "x \"" << itemName << "\" to order #" << orderId << ">\n";
}

void zharov::orderRemove(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string queueName;
  in >> queueName;
  if (!cafe.queues.has(queueName))
  {
    throw std::invalid_argument("Queue not found");
  }
  int orderId = 0;
  in >> orderId;
  if (!cafe.queues.at(queueName).has(orderId))
  {
    throw std::invalid_argument("Order not found");
  }
  std::string itemName;
  in >> itemName;
  zharov::Order& order = cafe.queues.at(queueName).at(orderId);
  if (!order.items.has(itemName))
  {
    throw std::invalid_argument("Item not found in order");
  }
  order.items.remove(itemName);
  out << "<OK: Removed \"" << itemName << "\" from order #" << orderId << ">\n";
}

namespace
{
  void printOrder(std::ostream& out,
    int orderId,
    const zharov::Order& order,
    const zharov::CafeSystem& cafe)
  {
    out << "=== ORDER #" << orderId << " ===\n";
    int total = 0, totalTime = 0;
    for (auto it = order.items.cbegin(); it != order.items.cend(); ++it)
    {
      int count = it->second;
      int price = 0, prepTime = 0;
      for (auto mit = cafe.menus.cbegin(); mit != cafe.menus.cend(); ++mit)
      {
        if (mit->second.has(it->first))
        {
          price = mit->second.at(it->first).price;
          prepTime = mit->second.at(it->first).prep_time;
          break;
        }
      }
      out << count << "x " << it->first << " - " << count * price << " rub - " << count * prepTime
          << " min\n";
      total += count * price;
      totalTime += count * prepTime;
    }
    out << "TOTAL: " << total << " rub\n";
    out << "TOTAL TIME: " << totalTime << " min\n";
  }
}

void zharov::orderShow(std::ostream& out, std::istream& in, const zharov::CafeSystem& cafe)
{
  std::string queueName;
  in >> queueName;
  if (!cafe.queues.has(queueName))
  {
    throw std::invalid_argument("Queue not found");
  }
  const zharov::QueueTable& queue = cafe.queues.at(queueName);
  if (hasMoreArgs(in))
  {
    int orderId = 0;
    in >> orderId;
    if (!queue.has(orderId))
    {
      throw std::invalid_argument("Order not found");
    }
    printOrder(out, orderId, queue.at(orderId), cafe);
  }
  else
  {
    for (auto it = queue.cbegin(); it != queue.cend(); ++it)
    {
      printOrder(out, it->first, it->second, cafe);
    }
  }
}

void zharov::orderCancel(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string queueName;
  in >> queueName;
  if (!cafe.queues.has(queueName))
  {
    throw std::invalid_argument("Queue not found");
  }
  int orderId = 0;
  in >> orderId;
  if (!cafe.queues.at(queueName).has(orderId))
  {
    throw std::invalid_argument("Order not found");
  }
  zharov::HistoryEntry entry;
  entry.queue_name = queueName;
  entry.order_id = orderId;
  entry.status = "cancelled";
  entry.order = cafe.queues.at(queueName).at(orderId);
  zharov::writeHistory(cafe, entry);
  cafe.queues.at(queueName).remove(orderId);
  out << "<OK: Order #" << orderId << " cancelled>\n";
}
