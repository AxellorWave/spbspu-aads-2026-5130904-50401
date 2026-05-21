#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <cstddef>
#include <stdexcept>
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
    detail::Node< Key, Value >* findNode(const Key& k) const;
    Value get(const Key& k) const;
    Value& at(const Key& k);
    const Value& at(const Key& k) const;
    detail::Node< Key, Value >* fallLeft(detail::Node< Key, Value >* node) const;
    Value drop(const Key& k);
    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);
    size_t height(const_iterator it) const;
    size_t height() const;

  private:
    detail::Node< Key, Value >* root_;
    size_t size_;
    Compare comp_;

    detail::Node< Key, Value >* initFake();
    detail::Node< Key, Value >* clone(detail::Node< Key, Value >* root,
      detail::Node< Key, Value >* parent);
    void deleteNodes(detail::Node< Key, Value >* node) noexcept;
    template < class K, class V >
    void pushImpl(K&& k, V&& v);
    size_t getHight(detail::Node< Key, Value >* node) const;
  };
}

template < class Key, class Value, class Compare >
zharov::detail::Node< Key, Value >* zharov::BSTree< Key, Value, Compare >::initFake()
{
  using Node = detail::Node< Key, Value >;
  if (Node::fake == nullptr)
  {
    Node::fake = new detail::Node< Key, Value >(Key(), Value(), nullptr);
    Node::fake->left_ = detail::Node< Key, Value >::fake;
    Node::fake->right_ = detail::Node< Key, Value >::fake;
    Node::fake->parent_ = detail::Node< Key, Value >::fake;
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
  if (this != std::addressof(other))
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
  if (this != std::addressof(other))
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

  Node* curr = root_;
  while (true)
  {
    if (comp_(k, curr->key_))
    {
      if (curr->left_->isFake())
      {
        curr->left_ = new Node(std::forward< K >(k), std::forward< V >(v), curr);
        ++size_;
        return;
      }
      curr = curr->left_;
    }
    else if (comp_(curr->key_, k))
    {
      if (curr->right_->isFake())
      {
        curr->right_ = new Node(std::forward< K >(k), std::forward< V >(v), curr);
        ++size_;
        return;
      }
      curr = curr->right_;
    }
    else
    {
      curr->value_ = std::forward< V >(v);
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

template < class Key, class Value, class Compare >
zharov::detail::Node< Key, Value >* zharov::BSTree< Key, Value, Compare >::findNode(
  const Key& k) const
{
  detail::Node< Key, Value >* curr = root_;
  while (!curr->isFake())
  {
    if (comp_(k, curr->key_))
    {
      curr = curr->left_;
    }
    else if (comp_(curr->key_, k))
    {
      curr = curr->right_;
    }
    else
    {
      return curr;
    }
  }
  return nullptr;
}

template < class Key, class Value, class Compare >
Value zharov::BSTree< Key, Value, Compare >::get(const Key& k) const
{
  return at(k);
}

template < class Key, class Value, class Compare >
Value& zharov::BSTree< Key, Value, Compare >::at(const Key& k)
{
  const BSTree< Key, Value, Compare >* const_tree = this;
  return const_cast< Value& >((*const_tree).at(k));
}

template < class Key, class Value, class Compare >
const Value& zharov::BSTree< Key, Value, Compare >::at(const Key& k) const
{
  const detail::Node< Key, Value >* node = findNode(k);
  if (node == nullptr)
  {
    throw std::logic_error("Key no found");
  }
  return node->value_;
}

template < class Key, class Value, class Compare >
zharov::detail::Node< Key, Value >* zharov::BSTree< Key, Value, Compare >::fallLeft(
  detail::Node< Key, Value >* node) const
{
  while (!node->left_->isFake())
  {
    node = node->left_;
  }
  return node;
}

template < class Key, class Value, class Compare >
Value zharov::BSTree< Key, Value, Compare >::drop(const Key& k)
{
  detail::Node< Key, Value >* node = findNode(k);
  if (node == nullptr)
  {
    throw std::out_of_range("Key not found");
  }

  Value res = std::move(node->value_);
  if (!node->left_->isFake() && !node->right_->isFake())
  {
    detail::Node< Key, Value >* n = fallLeft(node->right_);
    node->key_ = std::move(n->key_);
    node->value_ = std::move(n->value_);
    node = n;
  }

  detail::Node< Key, Value >* child = (!node->left_->isFake()) ? node->left_ : node->right_;
  if (!child->isFake())
  {
    child->parent_ = node->parent_;
  }

  if (node->parent_->isFake())
  {
    root_ = child;
  }
  else if (node->parent_->left_ == node)
  {
    node->parent_->left_ = child;
  }
  else
  {
    node->parent_->right_ = child;
  }

  delete node;
  --size_;
  return res;
}

template < class Key, class Value, class Compare >
size_t zharov::BSTree< Key, Value, Compare >::getHight(detail::Node< Key, Value >* node) const
{
  if (node->isFake())
  {
    return 0;
  }
  size_t left = getHight(node->left_);
  size_t right = getHight(node->right_);
  return 1 + (left > right ? left : right);
}

template < class Key, class Value, class Compare >
size_t zharov::BSTree< Key, Value, Compare >::height() const
{
  return getHight(root_);
}

#endif
