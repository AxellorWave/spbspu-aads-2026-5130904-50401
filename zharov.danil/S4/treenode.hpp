#ifndef TREENODE_HPP
#define TREENODE_HPP
#include <utility>

namespace zharov
{
  namespace detail
  {
    template < class Key, class Value >
    struct Node
    {
      std::pair< const Key, Value > data_;
      Node< Key, Value >* left_;
      Node< Key, Value >* right_;
      Node< Key, Value >* parent_;

      static Node< Key, Value >* fake;

      Node(const Key& key, const Value& val, Node< Key, Value >* parent);
      Node(Key&& key, Value&& val, Node< Key, Value >* parent);

      bool isFake() const;
    };

    template < class Key, class Value >
    detail::Node< Key, Value >* detail::Node< Key, Value >::fake = nullptr;
  }
}

template < class Key, class Value >
zharov::detail::Node< Key, Value >::Node(const Key& key,
  const Value& val,
  Node< Key, Value >* parent):
  data_(key, val),
  left_(fake),
  right_(fake),
  parent_(parent)
{}

template < class Key, class Value >
zharov::detail::Node< Key, Value >::Node(Key&& key, Value&& val, Node< Key, Value >* parent):
  data_(key, val),
  left_(fake),
  right_(fake),
  parent_(parent)
{}

template < class Key, class Value >
bool zharov::detail::Node< Key, Value >::isFake() const
{
  return this == fake;
}

#endif
