#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <string>
#include <hash_table.hpp>
#include <vector.hpp>
#include "hasher.hpp"

namespace zharov
{
  struct Graph
  {
    using key_t = std::pair< std::string, std::string >;
    struct VertComp
    {
      bool operator()(const key_t& v1, const key_t& v2) const;
    };

    void addVertex(const std::string& v);
    void rmVertex(const std::string& v);
    void addEdge(const std::string& v1, const std::string& v2, size_t w);
    void rmEdge(const std::string& v1, const std::string& v2, size_t w);

    zharov::HashTable< key_t, size_t, Blake2Hasher< key_t >, VertComp > edges_;
    zharov::Vector< std::string > vertexes_;
  };
}

#endif
