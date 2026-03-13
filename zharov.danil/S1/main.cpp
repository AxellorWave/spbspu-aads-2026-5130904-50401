#include <iostream>
#include "list.hpp"
#include <string>

namespace zharov {
  std::istream & getData(std::istream & in,
    List< std::pair< std::string, List< int > > > & data);
  std::ostream & printList(std::ostream & out, List< std::pair< std::string,
    List< int > > > data);
}

int main()
{
  zharov::List< std::pair< std::string, zharov::List< int > > > data;
  zharov::getData(std::cin, data);
  zharov::printList(std::cout, data);
}

std::istream & zharov::getData(std::istream & in,
  List< std::pair< std::string, List< int > > > & data)
{
  std::string name;
  while (in >> name) {
    int num;
    List< int > nums;
    while (in >> num) {
      nums.pushBack(num);
    }
    data.pushBack(make_pair(name, nums));
    in.clear();
  }
  return in;
}

std::ostream & zharov::printList(std::ostream & out, List< std::pair< std::string,
  List< int > > > data)
{
  for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
    out << it->first << " ";
    for (auto num_it = it->second.constBegin(); num_it != it->second.constEnd(); ++num_it) {
      out << *num_it << " ";
    }
    out << "\n";
  }
  return out;
}
