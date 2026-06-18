#include "optimize.hpp"
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <string>
#include "cafe.hpp"
#include "order.hpp"
#include <vector.hpp>

void zharov::cmdOptimize(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string srcName, acceptName, rejectName;
  int availableTime = 0;
  in >> srcName;
  if (!cafe.queues.has(srcName))
  {
    throw std::invalid_argument("Source queue not found");
  }
  in >> acceptName;
  if (cafe.queues.has(acceptName))
  {
    throw std::invalid_argument("Accepted queue already exists");
  }
  in >> rejectName;
  if (cafe.queues.has(rejectName))
  {
    throw std::invalid_argument("Rejected queue already exists");
  }
  in >> availableTime;
  if (availableTime <= 0)
  {
    throw std::invalid_argument("Available time must be positive");
  }

  const zharov::QueueTable& src = cafe.queues.at(srcName);
  size_t n = src.size();

  zharov::Vector< int > profits(n, 0);
  zharov::Vector< int > times(n, 0);
  zharov::Vector< int > orderIds(n, 0);

  size_t idx = 0;
  for (auto it = src.cbegin(); it != src.cend(); ++it, ++idx)
  {
    orderIds[idx] = it->first;
    profits[idx] = it->second.getProfit(cafe);
    times[idx] = it->second.getTime(cafe);
  }

  zharov::Vector< zharov::Vector< int > > dp(n + 1, zharov::Vector< int >(availableTime + 1, 0));

  for (size_t i = 1; i <= n; ++i)
  {
    for (int w = 0; w <= availableTime; ++w)
    {
      dp[i][w] = dp[i - 1][w];
      if (times[i - 1] <= w)
      {
        dp[i][w] = std::max(dp[i][w], dp[i - 1][w - times[i - 1]] + profits[i - 1]);
      }
    }
  }

  zharov::Vector< bool > selected(n, false);
  int w = availableTime;
  for (size_t i = n; i >= 1; --i)
  {
    if (dp[i][w] != dp[i - 1][w])
    {
      selected[i - 1] = true;
      w -= times[i - 1];
    }
  }

  cafe.queues.add(acceptName, zharov::QueueTable{});
  cafe.queues.add(rejectName, zharov::QueueTable{});

  std::ios::fmtflags savedFlags = out.flags();
  out << std::right;

  out << "SOURCE QUEUE: " << srcName << "\n";
  out << "AVAILABLE TIME: " << availableTime << " min\n";
  out << "PROCESSING QUEUE:\n";
  int totalProfit = 0;
  for (size_t i = 0; i < n; ++i)
  {
    const zharov::Order& order = src.at(orderIds[i]);
    if (selected[i])
    {
      cafe.queues.at(acceptName).add(orderIds[i], order);
      out << "#" << std::setw(3) << orderIds[i] << " | " << std::setw(5) << profits[i] << " rub"
          << " | " << std::setw(3) << times[i] << " min\n";
      totalProfit += profits[i];
    }
  }
  out << "TOTAL PROFIT: " << totalProfit << " rub\n";
  out << "REJECTED QUEUE:\n";
  for (size_t i = 0; i < n; ++i)
  {
    const zharov::Order& order = src.at(orderIds[i]);
    if (!selected[i])
    {
      cafe.queues.at(rejectName).add(orderIds[i], order);
      out << "#" << std::setw(3) << orderIds[i] << " | " << std::setw(5) << profits[i] << " rub"
          << " | " << std::setw(3) << times[i] << " min\n";
    }
  }

  out.flags(savedFlags);
  cafe.queues.remove(srcName);
}
