#include "graph.hpp"

zharov::Graph::Graph(size_t count):
  edges_(count),
  vertexes_()
{}

void zharov::Graph::addVertex(const std::string& v)
{
  if (vertexes_.contains(v))
  {
    return;
  }
  vertexes_.pushBack(v);
}

void zharov::Graph::rmVertex(const std::string& v)
{
  if (!vertexes_.contains(v))
  {
    return;
  }

  for (auto i = vertexes_.begin(); i != vertexes_.end(); ++i)
  {
    if (v == *i)
    {
      vertexes_.erase(i);
      break;
    }
  }

  for (auto i = edges_.begin(); i != edges_.end(); ++i)
  {
    if (i->first.first == v || i->first.second == v)
    {
      edges_.remove(i->first);
    }
  }
}

void zharov::Graph::addEdge(const std::string& v1, const std::string& v2, size_t w)
{
  addVertex(v1);
  addVertex(v2);
  auto key = std::make_pair(v1, v2);
  if (edges_.contains(key))
  {
    edges_.at(key).pushBack(w);
  }
  else
  {
    try
    {
      edges_.add(key, Vector< size_t >{w});
    }
    catch (...)
    {
      edges_.rehash();
      edges_.add(key, Vector< size_t >{w});
    }
  }
}

void zharov::Graph::rmEdge(const std::string& v1, const std::string& v2, size_t w)
{
  auto key = std::make_pair(v1, v2);
  if (edges_.contains(key))
  {
    auto& v = edges_.at(key);
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
      edges_.remove(key);
    }
  }
}
