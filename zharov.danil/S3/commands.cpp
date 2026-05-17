#include "commands.hpp"

bool zharov::KeyComp::operator()(const std::string& k1, const std::string& k2) const
{
  return k1 == k2;
}

void zharov::graphs(std::ostream& out, std::istream&, const graphs_table& graphs)
{
  if (graphs.getSize() == 0)
  {
    out << "\n";
  }
  for (auto i = graphs.begin(); i != graphs.end(); ++i)
  {
    out << i->key_ << "\n";
  }
}

void zharov::vertexes(std::ostream&, std::istream&, const graphs_table&)
{}

void zharov::outbound(std::ostream&, std::istream&, const graphs_table&)
{}

void zharov::inbound(std::ostream&, std::istream&, const graphs_table&)
{}

void zharov::bind(std::ostream&, std::istream&, graphs_table&)
{}

void zharov::cut(std::ostream&, std::istream&, graphs_table&)
{}

void zharov::create(std::ostream&, std::istream&, graphs_table&)
{}

void zharov::merge(std::ostream&, std::istream&, graphs_table&)
{}

void zharov::extract(std::ostream&, std::istream&, graphs_table&)
{}
