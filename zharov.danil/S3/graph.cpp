#include "graph.hpp"

zharov::Graph::Graph(size_t count):
  edges(count),
  vertexes()
{}

void zharov::Graph::addVertex(const std::string& v)
{
  if (vertexes.has(v))
  {
    return;
  }
  vertexes.pushBack(v);
}

void zharov::Graph::rmVertex(const std::string& v)
{
  if (!vertexes.has(v))
  {
    return;
  }

  for (auto i = vertexes.begin(); i != vertexes.end(); ++i)
  {
    if (v == *i)
    {
      vertexes.erase(i);
      break;
    }
  }

  for (auto i = edges.begin(); i != edges.end(); ++i)
  {
    if (i->first.first == v || i->first.second == v)
    {
      edges.remove(i->first);
    }
  }
}

void zharov::Graph::addEdge(const std::string& v1, const std::string& v2, size_t w)
{
  addVertex(v1);
  addVertex(v2);
  zharov::key_t key = std::make_pair(v1, v2);
  if (edges.contains(key))
  {
    edges.at(key).pushBack(w);
  }
  else
  {
    try
    {
      edges.add(key, Vector< size_t >{w});
    }
    catch (...)
    {
      edges.rehash();
      edges.add(key, Vector< size_t >{w});
    }
  }
}

void zharov::Graph::rmEdge(const std::string& v1, const std::string& v2, size_t w)
{
  zharov::key_t key = std::make_pair(v1, v2);
  if (edges.contains(key))
  {
    zharov::Vector< size_t >& v = edges.at(key);
    if (v.getSize() > 1)
    {
      for (auto i = v.begin(); i != v.end(); ++i)
      {
        if (*i == w)
        {
          v.erase(i);
          return;
        }
      }
    }
    else
    {
      edges.remove(key);
    }
  }
}
