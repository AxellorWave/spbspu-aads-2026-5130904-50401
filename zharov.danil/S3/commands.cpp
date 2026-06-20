#include <functional>
#include "commands.hpp"

namespace
{
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
}

void zharov::graphs(std::ostream& out, std::istream&, const graphs_table& graphs)
{
  if (graphs.size() == 0)
  {
    out << "\n";
    return;
  }
  zharov::Vector< std::string > names;
  for (auto i = graphs.begin(); i != graphs.end(); ++i)
  {
    names.pushBack(i->first);
  }
  sort(names, std::less< std::string >{});
  for (auto i = names.begin(); i != names.end(); ++i)
  {
    out << *i << "\n";
  }
}

void zharov::vertexes(std::ostream& out, std::istream& in, const graphs_table& graphs)
{
  std::string gr_name;
  in >> gr_name;
  if (!graphs.contains(gr_name))
  {
    throw std::logic_error("Graph not found");
  }

  const zharov::Graph& gr = graphs.at(gr_name);
  zharov::Vector< std::string > names;
  for (auto i = gr.vertexes_.cbegin(); i != gr.vertexes_.cend(); ++i)
  {
    names.pushBack(*i);
  }

  sort(names, std::less< std::string >{});
  if (names.isEmpty())
  {
    out << "\n";
  }
  for (auto i = names.begin(); i != names.end(); ++i)
  {
    out << *i << "\n";
  }
}

void zharov::outbound(std::ostream& out, std::istream& in, const graphs_table& graphs)
{
  std::string gr_name, vert_name;
  in >> gr_name >> vert_name;
  if (!graphs.contains(gr_name))
  {
    throw std::logic_error("Graph not found");
  }

  const zharov::Graph& gr = graphs.at(gr_name);
  if (!gr.vertexes_.has(vert_name))
  {
    throw std::logic_error("Vertex not found");
  }

  zharov::Vector< std::pair< std::string, size_t > > names;
  for (auto i = gr.edges_.cbegin(); i != gr.edges_.cend(); ++i)
  {
    if (i->first.first == vert_name)
    {
      for (auto j = i->second.begin(); j != i->second.end(); ++j)
      {
        names.pushBack(std::make_pair(i->first.second, *j));
      }
    }
  }
  if (names.isEmpty())
  {
    out << "\n";
  }
  sort(names, std::less< std::pair< std::string, size_t > >{});
  for (auto i = names.begin(); i != names.end();)
  {
    std::string temp = i->first;
    out << i->first << " " << i->second;
    ++i;
    while (i != names.end() && i->first == temp)
    {
      out << " " << i->second;
      ++i;
    }
    out << "\n";
  }
}

void zharov::inbound(std::ostream& out, std::istream& in, const graphs_table& graphs)
{
  std::string gr_name, vert_name;
  in >> gr_name >> vert_name;
  if (!graphs.contains(gr_name))
  {
    throw std::logic_error("Graph not found");
  }

  const zharov::Graph& gr = graphs.at(gr_name);
  if (!gr.vertexes_.has(vert_name))
  {
    throw std::logic_error("Vertex not found");
  }

  zharov::Vector< std::pair< std::string, size_t > > names;
  for (auto i = gr.edges_.cbegin(); i != gr.edges_.cend(); ++i)
  {
    if (i->first.second == vert_name)
    {
      for (auto j = i->second.begin(); j != i->second.end(); ++j)
      {
        names.pushBack(std::make_pair(i->first.first, *j));
      }
    }
  }
  if (names.isEmpty())
  {
    out << "\n";
  }
  sort(names, std::less< std::pair< std::string, size_t > >{});
  for (auto i = names.begin(); i != names.end();)
  {
    std::string temp = i->first;
    out << i->first << " " << i->second;
    ++i;
    while (i != names.end() && i->first == temp)
    {
      out << " " << i->second;
      ++i;
    }
    out << "\n";
  }
}

void zharov::bind(std::ostream&, std::istream& in, graphs_table& graphs)
{
  std::string gr_name, vert_name_1, vert_name_2;
  size_t weight;
  in >> gr_name >> vert_name_1 >> vert_name_2 >> weight;
  if (!graphs.contains(gr_name))
  {
    throw std::logic_error("Graph not found");
  }

  graphs.at(gr_name).addEdge(vert_name_1, vert_name_2, weight);
}

void zharov::cut(std::ostream&, std::istream& in, graphs_table& graphs)
{
  std::string gr_name, vert_name_1, vert_name_2;
  size_t weight;
  in >> gr_name >> vert_name_1 >> vert_name_2 >> weight;
  if (!graphs.contains(gr_name))
  {
    throw std::logic_error("Graph not found");
  }
  zharov::Graph& gr = graphs.at(gr_name);
  if (!gr.vertexes_.has(vert_name_1) || !gr.vertexes_.has(vert_name_2))
  {
    throw std::logic_error("Vertex not found");
  }

  if (!gr.edges_.contains(std::make_pair(vert_name_1, vert_name_2)) ||
    !gr.edges_.at(std::make_pair(vert_name_1, vert_name_2)).has(weight))
  {
    throw std::logic_error("Edge not found");
  }
  gr.rmEdge(vert_name_1, vert_name_2, weight);
}

void zharov::create(std::ostream&, std::istream& in, graphs_table& graphs)
{
  std::string gr_name, vertex;
  size_t count;
  in >> gr_name;
  if (graphs.contains(gr_name))
  {
    throw std::logic_error("Graph already exist");
  }
  in >> count;
  Graph gr(count);
  for (size_t i = 0; i < count; ++i)
  {
    in >> vertex;
    gr.addVertex(vertex);
  }
  graphs.add(gr_name, gr);
  if (in.fail())
  {
    in.clear();
  }
}

void zharov::merge(std::ostream&, std::istream& in, graphs_table& graphs)
{
  std::string gr_new, gr_old_1, gr_old_2;
  in >> gr_new >> gr_old_1 >> gr_old_2;
  if (graphs.contains(gr_new))
  {
    throw std::logic_error("Graph already exist");
  }
  if (!graphs.contains(gr_old_1) || !graphs.contains(gr_old_2))
  {
    throw std::logic_error("Graph not found");
  }

  Graph gr;
  gr.vertexes_ = graphs.at(gr_old_1).vertexes_;
  gr.edges_ = graphs.at(gr_old_1).edges_;
  for (auto i = graphs.at(gr_old_2).vertexes_.cbegin(); i != graphs.at(gr_old_2).vertexes_.cend();
    ++i)
  {
    gr.addVertex(*i);
  }
  for (auto i = graphs.at(gr_old_2).edges_.cbegin(); i != graphs.at(gr_old_2).edges_.cend(); ++i)
  {
    for (auto j = i->second.cbegin(); j != i->second.cend(); ++j)
    {
      gr.addEdge(i->first.first, i->first.second, *j);
    }
  }

  graphs.add(gr_new, gr);
}

void zharov::extract(std::ostream&, std::istream& in, graphs_table& graphs)
{
  std::string gr_new, gr_old, vertex;
  size_t count;
  in >> gr_new >> gr_old >> count;
  Vector< std::string > vertexes;
  if (graphs.contains(gr_new))
  {
    throw std::logic_error("Graph already exist");
  }
  if (!graphs.contains(gr_old))
  {
    throw std::logic_error("Graph not found");
  }

  for (size_t i = 0; i < count; ++i)
  {
    in >> vertex;
    vertexes.pushBack(vertex);
  }
  for (auto i = vertexes.cbegin(); i != vertexes.cend(); ++i)
  {
    if (!graphs.at(gr_old).vertexes_.has(*i))
    {
      throw std::logic_error("Vertex not found");
    }
  }

  Graph gr;
  gr.vertexes_ = vertexes;
  for (auto i = graphs.at(gr_old).edges_.cbegin(); i != graphs.at(gr_old).edges_.cend(); ++i)
  {
    if (vertexes.has(i->first.first) && vertexes.has(i->first.second))
    {
      for (auto j = i->second.cbegin(); j != i->second.cend(); ++j)
      {
        gr.addEdge(i->first.first, i->first.second, *j);
      }
    }
  }
  graphs.add(gr_new, gr);
}
