#include "commands.hpp"

void zharov::print(std::ostream& out, std::istream& in, dictionaries_t& dicts)
{
  std::string dict_name;
  in >> dict_name;
  if (!dicts.has(dict_name))
  {
    throw std::logic_error("Graph not found");
  }

  out << dict_name;
  for (auto i = dicts.at(dict_name).cbegin(); i != dicts.at(dict_name).cend(); ++i)
  {
    out << ' ' << i->first << ' ' << i->second;
  }
  out << "\n";
}

void zharov::complement(std::ostream&, std::istream&, dictionaries_t&)
{}

void zharov::intersect(std::ostream&, std::istream&, dictionaries_t&)
{}

void zharov::makeUnion(std::ostream&, std::istream&, dictionaries_t&)
{}
