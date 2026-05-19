#include <iostream>
#include <string>
#include <limits>
#include <list.hpp>

namespace
{
  std::istream& getData(std::istream& in,
    zharov::List< std::pair< std::string, zharov::List< size_t > > >& data);
  std::ostream& printNums(std::ostream& out, zharov::List< zharov::List< size_t > > nums);
  std::ostream& printNames(std::ostream& out,
    zharov::List< std::pair< std::string, zharov::List< size_t > > > data);
  zharov::List< zharov::List< size_t > > transposeNums(
    zharov::List< std::pair< std::string, zharov::List< size_t > > > data);
  size_t getSize(zharov::List< std::pair< std::string, zharov::List< size_t > > > data);
  zharov::List< size_t > getSums(zharov::List< zharov::List< size_t > > nums);
  std::ostream& printSums(std::ostream& out, zharov::List< size_t > sums);
}

int main()
{
  zharov::List< std::pair< std::string, zharov::List< size_t > > > data;
  getData(std::cin, data);
  if (data.cbegin() == data.cend())
  {
    std::cout << "0\n";
    return 0;
  }
  printNames(std::cout, data);
  std::cout << "\n";
  if (getSize(data) == 0)
  {
    std::cout << "0\n";
    return 0;
  }
  auto transpose_nums = transposeNums(data);
  printNums(std::cout, transpose_nums);
  std::cout << "\n";
  try
  {
    auto sums_list = getSums(transpose_nums);
    printSums(std::cout, sums_list);
    std::cout << "\n";
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}

namespace
{
  std::istream& getData(std::istream& in,
    zharov::List< std::pair< std::string, zharov::List< size_t > > >& data)
  {
    std::string name;
    while (in >> name)
    {
      size_t num;
      zharov::List< size_t > nums;
      while (in >> num)
      {
        nums.pushBack(num);
      }
      data.pushBack(make_pair(name, nums));
      in.clear();
    }
    return in;
  }

  std::ostream& printNums(std::ostream& out, zharov::List< zharov::List< size_t > > nums)
  {
    for (auto it = nums.cbegin(); it != nums.cend(); ++it)
    {
      auto inner_it = it->cbegin();
      out << *inner_it;
      ++inner_it;
      for (; inner_it != it->cend(); ++inner_it)
      {
        out << ' ';
        out << *inner_it;
      }
    }
    return out;
  }

  std::ostream& printNames(std::ostream& out,
    zharov::List< std::pair< std::string, zharov::List< size_t > > > data)
  {
    auto it = data.cbegin();
    out << it->first;
    ++it;
    for (; it != data.cend(); ++it)
    {
      std::cout << ' ';
      out << it->first;
    }
    return out;
  }

  size_t getSize(zharov::List< std::pair< std::string, zharov::List< size_t > > > data)
  {
    size_t res = 0;
    for (auto it = data.cbegin(); it != data.cend(); ++it)
    {
      res = std::max(res, it->second.size());
    }
    return res;
  }

  zharov::List< zharov::List< size_t > > transposeNums(
    zharov::List< std::pair< std::string, zharov::List< size_t > > > data)
  {
    zharov::List< zharov::List< size_t > > res;
    for (size_t i = 0; i < getSize(data); ++i)
    {
      zharov::List< size_t > inner;
      for (auto it = data.cbegin(); it != data.cend(); ++it)
      {
        auto num_it = it->second.cbegin();
        if (it->second.size() > i)
        {
          for (size_t j = 0; j < i; ++j)
          {
            ++num_it;
          }
          inner.pushBack(*num_it);
        }
      }
      res.pushBack(inner);
    }
    return res;
  }

  zharov::List< size_t > getSums(zharov::List< zharov::List< size_t > > nums)
  {
    zharov::List< size_t > res;
    constexpr size_t MAX = std::numeric_limits< size_t >::max();
    for (auto it = nums.cbegin(); it != nums.cend(); ++it)
    {
      size_t sum = 0;
      for (auto inner_it = it->cbegin(); inner_it != it->cend(); ++inner_it)
      {
        if (MAX - *inner_it < sum)
        {
          throw std::overflow_error("Overflow");
        }
        sum += *inner_it;
      }
      res.pushBack(sum);
    }
    return res;
  }

  std::ostream& printSums(std::ostream& out, zharov::List< size_t > sums)
  {
    auto it = sums.cbegin();
    out << *it;
    ++it;
    for (; it != sums.cend(); ++it)
    {
      out << ' ';
      out << *it;
    }
    return out;
  }
}
