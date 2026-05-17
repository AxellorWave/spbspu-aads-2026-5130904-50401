#include "graph.hpp"

bool zharov::VertComp::operator()(const key_t& v1, const key_t& v2) const
{
  return v1.first == v2.first && v1.second == v2.second;
}

zharov::Graph::Graph(size_t count):
  edges_(count),
  vertexes_()
{}

void zharov::Graph::addVertex(const std::string& v)
{
  if (vertexes_.has(v))
  {
    return;
  }
  vertexes_.pushBack(v);
}

void zharov::Graph::rmVertex(const std::string& v)
{
  if (!vertexes_.has(v))
  {
    return;
  }

  for (auto i = vertexes_.begin(); i != vertexes_.end(); ++i)
  {
    if (v == *i)
    {
      vertexes_.erase(i);
      return;
    }
  }

  for (auto i = edges_.begin(); i != edges_.end(); ++i)
  {
    if (i->key_.first == v || i->key_.second == v)
    {
      edges_.remove(i->key_);
    }
  }
}

void zharov::Graph::addEdge(const std::string& v1, const std::string& v2, size_t w)
{
  addVertex(v1);
  addVertex(v2);
  auto key = std::make_pair(v1, v2);
  if (edges_.has(key))
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
  if (edges_.has(key))
  {
    auto v = edges_.at(key);
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
