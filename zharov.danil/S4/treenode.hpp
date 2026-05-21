#ifndef TREENODE_HPP
#define TREENODE_HPP

namespace zharov
{
  namespace detail
  {
    template < class Key, class Value >
    struct Node
    {
      Key key_;
      Value value_;
      Node< Key, Value > left_;
      Node< Key, Value > right_;
      Node< Key, Value > parent_;

      static Node< Key, Value >* fake;

      Node(const Key& key, const Value& val, Node< Key, Value > parent);
      Node(Key&& key, Value&& val, Node< Key, Value > parent);

      bool isFake() const;
    };
  }
}

#endif
