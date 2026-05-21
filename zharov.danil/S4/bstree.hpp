#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <cstddef>
#include "treenode.hpp"

namespace zharov
{
  template < class Key, class Value >
  class BSTIterator
  {
  };
  template < class Key, class Value >
  class BSTConstIterator
  {
  };

  template < class Key, class Value, class Compare >
  class BSTree
  {
  public:
    using const_iterator = BSTConstIterator< Key, Value >;
    using iterator = BSTIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;
    ~BSTree() noexcept;

    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;

    size_t getSize() const;
    void swap(BSTree& other) noexcept;
    void clear() noexcept;

    void push(Key k, Value v);
    Value get(Key k);
    Value drop(Key k);
    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);
    size_t height(const_iterator it);
    size_t height();

  private:
    detail::Node< Key, Value >* root_;
    size_t size_;
    Compare comp_;

    detail::Node< Key, Value >* initFake();
    detail::Node< Key, Value >* BSTree< Key, Value, Compare >::clone(
      detail::Node< Key, Value >* root,
      detail::Node< Key, Value >* parent)
  };
}

template < class Key, class Value, class Compare >
zharov::detail::Node< Key, Value >* zharov::BSTree< Key, Value, Compare >::initFake()
{
  if (detail::Node< Key, Value >::fake == nullptr)
  {
    detail::Node< Key, Value >::fake = new detail::Node< Key, Value >(Key(), Value(), nullptr);
    detail::Node< Key, Value >::fake->left_ = etail::Node< Key, Value >::fake;
    detail::Node< Key, Value >::fake->right_ = etail::Node< Key, Value >::fake;
    detail::Node< Key, Value >::fake->parent_ = etail::Node< Key, Value >::fake;
  }
  return detail::Node< Key, Value >::fake;
}

template < class Key, class Value, class Compare >
zharov::BSTree< Key, Value, Compare >::BSTree():
  root_(initFake()),
  size_(0),
  comp_()
{}

template < class Key, class Value, class Compare >
zharov::BSTree< Key, Value, Compare >::BSTree(const BSTree& other):
  root_(initFake()),
  size_(0),
  comp_(other.comp_)
{
  root_ = clone(other.root_, root_);
  size_ = other.size_;
}

template < class Key, class Value, class Compare >
zharov::BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept:
  root_(std::exchange(other.root_, detail::Node< Key, Value >::fake)),
  size_(std::exchange(other.size_, 0)),
  comp_(std::move(other.comp_))
{}

template < class Key, class Value, class Compare >
size_t zharov::BSTree< Key, Value, Compare >::getSize() const
{
  return size_;
}

template < class Key, class Value, class Compare >
zharov::detail::Node< Key, Value >* zharov::BSTree< Key, Value, Compare >::clone(
  detail::Node< Key, Value >* root,
  detail::Node< Key, Value >* parent)
{
  if (root->isFake())
  {
    return detail::Node< Key, Value >::fake;
  }
  detail::Node< Key, Value >* node =
    new detail::Node< Key, Value >(root->key_, root->value_, parent);
  node->left_ = clone(root->left_, node);
  node->right_ = clone(root->right_, node);
  return node;
}

#endif
