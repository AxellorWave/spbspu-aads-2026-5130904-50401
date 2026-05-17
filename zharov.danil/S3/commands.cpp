#include "commands.hpp"

template < class T >
struct SortComparator
{
  bool operator()(const T& p1, const T& p2)
  {
    return p1 < p2;
  }
};

template <>
struct SortComparator< std::pair< std::string, size_t > >
{
  using pair_t = std::pair< std::string, size_t >;
  bool operator()(const pair_t& p1, const pair_t& p2)
  {
    if (p1.first != p2.first)
    {
      return p1.first < p2.first;
    }
    return p1.second < p2.second;
  }
};

template < class T, class Cmp >
void sort(zharov::Vector< T >& v, Cmp cmp)
{
  for (size_t i = 0; i < v.getSize(); ++i)
  {
    size_t min = i;
    for (size_t j = i + 1; j < v.getSize(); ++j)
    {
      if (cmp(v[j], v[min]))
      {
        min = j;
      }
    }
    if (min != i)
    {
      std::swap(v[i], v[min]);
    }
  }
}

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
  zharov::Vector< std::string > names;
  for (auto i = graphs.begin(); i != graphs.end(); ++i)
  {
    names.pushBack(i->key_);
  }
  sort(names, SortComparator< std::string >{});
  for (auto i = names.begin(); i != names.end(); ++i)
  {
    out << *i << "\n";
  }
}

void zharov::vertexes(std::ostream& out, std::istream& in, const graphs_table& graphs)
{
  std::string gr_name;
  in >> gr_name;
  if (!graphs.has(gr_name))
  {
    throw std::logic_error("Graph not found");
  }

  zharov::Vector< std::string > names;
  for (auto i = graphs.at(gr_name).vertexes_.cbegin(); i != graphs.at(gr_name).vertexes_.cend();
    ++i)
  {
    names.pushBack(*i);
  }

  sort(names, SortComparator< std::string >{});
  for (auto i = names.begin(); i != names.end(); ++i)
  {
    out << *i << "\n";
  }
}

void zharov::outbound(std::ostream& out, std::istream& in, const graphs_table& graphs)
{
  std::string gr_name, vert_name;
  in >> gr_name >> vert_name;
  if (!graphs.has(gr_name))
  {
    throw std::logic_error("Graph not found");
  }

  if (!graphs.at(gr_name).vertexes_.has(vert_name))
  {
    throw std::logic_error("Vertex not found");
  }

  zharov::Vector< std::pair< std::string, size_t > > names;
  for (auto i = graphs.at(gr_name).edges_.cbegin(); i != graphs.at(gr_name).edges_.cend(); ++i)
  {
    if (i->key_.first == vert_name)
    {
      for (auto j = i->value_.begin(); j != i->value_.end(); ++j)
      {
        names.pushBack(std::make_pair(i->key_.second, *j));
      }
    }
  }

  sort(names, SortComparator< std::pair< std::string, size_t > >{});
  for (auto i = names.begin(); i != names.end(); ++i)
  {
    out << i->first << " " << i->second << "\n";
  }
}

void zharov::inbound(std::ostream& out, std::istream& in, const graphs_table& graphs)
{
  std::string gr_name, vert_name;
  in >> gr_name >> vert_name;
  if (!graphs.has(gr_name))
  {
    throw std::logic_error("Graph not found");
  }

  if (!graphs.at(gr_name).vertexes_.has(vert_name))
  {
    throw std::logic_error("Vertex not found");
  }

  zharov::Vector< std::pair< std::string, size_t > > names;
  for (auto i = graphs.at(gr_name).edges_.cbegin(); i != graphs.at(gr_name).edges_.cend(); ++i)
  {
    if (i->key_.second == vert_name)
    {
      for (auto j = i->value_.begin(); j != i->value_.end(); ++j)
      {
        names.pushBack(std::make_pair(i->key_.first, *j));
      }
    }
  }

  sort(names, SortComparator< std::pair< std::string, size_t > >{});
  for (auto i = names.begin(); i != names.end(); ++i)
  {
    out << i->first << " " << i->second << "\n";
  }
}

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
