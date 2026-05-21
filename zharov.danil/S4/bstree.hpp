#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <cstddef>
#include <utility>
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

    void push(const Key& k, const Value& v);
    void push(Key&& k, Value&& v);
    Value get(const Key& k);
    Value& at(const Key& k);
    const Value& at(const Key& k) const;
    Value drop(const Key& k);
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
      detail::Node< Key, Value >* parent);
    void BSTree< Key, Value, Compare >::deleteNodes(detail::Node< Key, Value >* node) noexcept;
    template < class K, class V >
    void pushImpl(K&& k, V&& v);
  };
}

template < class Key, class Value, class Compare >
zharov::detail::Node< Key, Value >* zharov::BSTree< Key, Value, Compare >::initFake()
{
  using Node = detail::Node< Key, Value >;
  if (Node::fake == nullptr)
  {
    Node::fake = new detail::Node< Key, Value >(Key(), Value(), nullptr);
    Node::fake->left_ = etail::Node< Key, Value >::fake;
    Node::fake->right_ = etail::Node< Key, Value >::fake;
    Node::fake->parent_ = etail::Node< Key, Value >::fake;
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
zharov::BSTree< Key, Value, Compare >::~BSTree() noexcept
{
  clear();
}

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
void zharov::BSTree< Key, Value, Compare >::deleteNodes(detail::Node< Key, Value >* node) noexcept
{
  if (node->isFake())
  {
    return;
  }
  deleteNodes(node->left_);
  deleteNodes(node->right_);
  delete node;
}

template < class Key, class Value, class Compare >
void zharov::BSTree< Key, Value, Compare >::clear() noexcept
{
  deleteNodes(root_);
  root_ = detail::Node< Key, Value >::fake;
  size_ = 0;
}

template < class Key, class Value, class Compare >
void zharov::BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
{
  std::swap(other.root_, root_);
  std::swap(other.size_, size_);
  std::swap(other.comp_, comp_);
}

template < class Key, class Value, class Compare >
zharov::detail::Node< Key, Value >* zharov::BSTree< Key, Value, Compare >::clone(
  detail::Node< Key, Value >* root,
  detail::Node< Key, Value >* parent)
{
  using Node = detail::Node< Key, Value >;
  if (root->isFake())
  {
    return Node::fake;
  }
  Node* node = new Node(root->key_, root->value_, parent);
  node->left_ = clone(root->left_, node);
  node->right_ = clone(root->right_, node);
  return node;
}

template < class Key, class Value, class Compare >
zharov::BSTree< Key, Value, Compare >& zharov::BSTree< Key, Value, Compare >::operator=(
  const BSTree& other)
{
  if (*this != std::addressof(other))
  {
    BSTree< Key, Value, Compare > copy(other);
    swap(copy);
  }
  return *this;
}

template < class Key, class Value, class Compare >
zharov::BSTree< Key, Value, Compare >& zharov::BSTree< Key, Value, Compare >::operator=(
  BSTree&& other) noexcept
{
  if (*this != std::addressof(other))
  {
    BSTree< Key, Value, Compare > copy(std::move(other));
    swap(copy);
  }
  return *this;
}

template < class Key, class Value, class Compare >
template < class K, class V >
void zharov::BSTree< Key, Value, Compare >::pushImpl(K&& k, V&& v)
{
  using Node = detail::Node< Key, Value >;

  if (root_->isFake())
  {
    root_ = new Node(std::forward< K >(k), std::forward< V >(v), Node::fake);
    ++size_;
    return;
  }

  Node* cur = root_;
  while (true)
  {
    if (comp_(k, cur->key_))
    {
      if (cur->left_->isFake())
      {
        cur->left_ = new Node(std::forward< K >(k), std::forward< V >(v), cur);
        ++size_;
        return;
      }
      cur = cur->left_;
    }
    else if (comp_(cur->key_, k))
    {
      if (cur->right_->isFake())
      {
        cur->right_ = new Node(std::forward< K >(k), std::forward< V >(v), cur);
        ++size_;
        return;
      }
      cur = cur->right_;
    }
    else
    {
      cur->value_ = std::forward< V >(v);
      return;
    }
  }
}

template < class Key, class Value, class Compare >
void zharov::BSTree< Key, Value, Compare >::push(const Key& k, const Value& v)
{
  pushImpl(k, v);
}

template < class Key, class Value, class Compare >
void zharov::BSTree< Key, Value, Compare >::push(Key&& k, Value&& v)
{
  pushImpl(std::move(k), std::move(v));
}

#endif
