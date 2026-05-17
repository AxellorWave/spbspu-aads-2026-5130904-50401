#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <string>
#include <fstream>
#include "graph.hpp"

namespace zharov
{
  struct KeyComp
  {
    bool operator()(const std::string& k1, const std::string& k2) const;
  };

  using graphs_table = HashTable< std::string, Graph, Blake2Hasher< std::string >, KeyComp >;
  void graphs(std::ostream&, std::istream&, const graphs_table&);
  void vertexes(std::ostream&, std::istream&, const graphs_table&);
  void outbound(std::ostream&, std::istream&, const graphs_table&);
  void inbound(std::ostream&, std::istream&, const graphs_table&);

  void bind(std::ostream&, std::istream&, graphs_table&);
  void cut(std::ostream&, std::istream&, graphs_table&);

  void create(std::ostream&, std::istream&, graphs_table&);
  void merge(std::ostream&, std::istream&, graphs_table&);
  void extract(std::ostream&, std::istream&, graphs_table&);
}

#endif
