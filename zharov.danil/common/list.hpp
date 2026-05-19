#ifndef LIST_HPP
#define LIST_HPP
#include <memory>
#include <cstddef>
#include <utility>

namespace zharov
{
  namespace detail
  {
    template < class T >
    struct Node
    {
      T val_;
      Node< T >* next_;
      Node< T >* prev_;
    };
  }
  template < class T >
  class List;

  template < class T >
  class LIter
  {
    friend class List< T >;

  public:
    T& operator*();
    T* operator->();
    LIter& operator++();
    LIter operator++(int);
    LIter& operator--();
    LIter operator--(int);
    bool operator==(const LIter& it) const;
    bool operator!=(const LIter& it) const;

  private:
    detail::Node< T >* curr_;
    LIter(detail::Node< T >* node);
  };

  template < class T >
  class LCIter
  {
    friend class List< T >;

  public:
    const T& operator*() const;
    const T* operator->() const;
    LCIter& operator++();
    LCIter operator++(int);
    LCIter& operator--();
    LCIter operator--(int);
    bool operator==(const LCIter& it) const;
    bool operator!=(const LCIter& it) const;

  private:
    const detail::Node< T >* curr_;
    LCIter(const detail::Node< T >* node);
  };

  template < class T >
  class List
  {
  public:
    List();
    List(const List& h);
    List(List&& h) noexcept;
    ~List();
    List& operator=(const List& h);
    List& operator=(List&& h) noexcept;
    LIter< T > begin();
    LIter< T > end();
    LCIter< T > cbegin() const;
    LCIter< T > cend() const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    void pushFront(const T& v);
    void pushBack(const T& v);
    LIter< T > insert(LIter< T > pos, const T& v);
    void popFront();
    void popBack();
    LIter< T > erase(LIter< T > pos);
    void clear();
    size_t size() const;

  private:
    detail::Node< T >* head_;
    detail::Node< T >* tail_;
    size_t size_;
  };
}

template < class T >
zharov::LIter< T >::LIter(detail::Node< T >* node):
  curr_(node)
{}

template < class T >
T& zharov::LIter< T >::operator*()
{
  return curr_->val_;
}

template < class T >
T* zharov::LIter< T >::operator->()
{
  return std::addressof(curr_->val_);
}

template < class T >
zharov::LIter< T >& zharov::LIter< T >::operator++()
{
  curr_ = curr_->next_;
  return *this;
}

template < class T >
zharov::LIter< T > zharov::LIter< T >::operator++(int)
{
  LIter< T > temp = *this;
  curr_ = curr_->next_;
  return temp;
}

template < class T >
zharov::LIter< T >& zharov::LIter< T >::operator--()
{
  curr_ = curr_->prev_;
  return *this;
}

template < class T >
zharov::LIter< T > zharov::LIter< T >::operator--(int)
{
  LIter< T > temp = *this;
  curr_ = curr_->prev_;
  return temp;
}

template < class T >
bool zharov::LIter< T >::operator==(const LIter& it) const
{
  return curr_ == it.curr_;
}

template < class T >
bool zharov::LIter< T >::operator!=(const LIter& it) const
{
  return curr_ != it.curr_;
}

template < class T >
zharov::LCIter< T >::LCIter(const detail::Node< T >* node):
  curr_(node)
{}

template < class T >
const T& zharov::LCIter< T >::operator*() const
{
  return curr_->val_;
}

template < class T >
const T* zharov::LCIter< T >::operator->() const
{
  return std::addressof(curr_->val_);
}

template < class T >
zharov::LCIter< T >& zharov::LCIter< T >::operator++()
{
  curr_ = curr_->next_;
  return *this;
}

template < class T >
zharov::LCIter< T > zharov::LCIter< T >::operator++(int)
{
  LCIter< T > temp = *this;
  curr_ = curr_->next_;
  return temp;
}

template < class T >
zharov::LCIter< T >& zharov::LCIter< T >::operator--()
{
  curr_ = curr_->prev_;
  return *this;
}

template < class T >
zharov::LCIter< T > zharov::LCIter< T >::operator--(int)
{
  LCIter< T > temp = *this;
  curr_ = curr_->prev_;
  return temp;
}

template < class T >
bool zharov::LCIter< T >::operator==(const LCIter& it) const
{
  return curr_ == it.curr_;
}

template < class T >
bool zharov::LCIter< T >::operator!=(const LCIter& it) const
{
  return curr_ != it.curr_;
}

template < class T >
zharov::List< T >::List():
  head_(nullptr),
  tail_(nullptr),
  size_(0)
{}

template < class T >
zharov::List< T >::List(const List< T >& h):
  List()
{
  for (detail::Node< T >* curr = h.head_; curr != nullptr; curr = curr->next_)
  {
    pushBack(curr->val_);
  }
}

template < class T >
zharov::List< T >::List(List< T >&& h) noexcept:
  head_(h.head_),
  tail_(h.tail_),
  size_(h.size_)
{
  h.head_ = nullptr;
  h.tail_ = nullptr;
  h.size_ = 0;
}

template < class T >
zharov::List< T >::~List()
{
  clear();
}

template < class T >
zharov::List< T >& zharov::List< T >::operator=(const List< T >& h)
{
  if (this != std::addressof(h))
  {
    List< T > temp(h);
    std::swap(temp.head_, head_);
    std::swap(temp.tail_, tail_);
    std::swap(temp.size_, size_);
  }
  return *this;
}

template < class T >
zharov::List< T >& zharov::List< T >::operator=(List< T >&& h) noexcept
{
  if (this != std::addressof(h))
  {
    clear();
    head_ = h.head_;
    tail_ = h.tail_;
    size_ = h.size_;
    h.head_ = nullptr;
    h.tail_ = nullptr;
    h.size_ = 0;
  }
  return *this;
}

template < class T >
zharov::LIter< T > zharov::List< T >::begin()
{
  return LIter< T >(head_);
}

template < class T >
zharov::LIter< T > zharov::List< T >::end()
{
  return LIter< T >(nullptr);
}

template < class T >
zharov::LCIter< T > zharov::List< T >::cbegin() const
{
  return LCIter< T >(head_);
}

template < class T >
zharov::LCIter< T > zharov::List< T >::cend() const
{
  return LCIter< T >(nullptr);
}

template < class T >
T& zharov::List< T >::front()
{
  return head_->val_;
}

template < class T >
const T& zharov::List< T >::front() const
{
  return head_->val_;
}

template < class T >
T& zharov::List< T >::back()
{
  return tail_->val_;
}

template < class T >
const T& zharov::List< T >::back() const
{
  return tail_->val_;
}

template < class T >
void zharov::List< T >::pushFront(const T& v)
{
  detail::Node< T >* new_node = new detail::Node< T >{v, nullptr, nullptr};
  new_node->next_ = head_;
  if (head_)
  {
    head_->prev_ = new_node;
  }
  else
  {
    tail_ = new_node;
  }
  head_ = new_node;
  ++size_;
}

template < class T >
void zharov::List< T >::pushBack(const T& v)
{
  detail::Node< T >* new_node = new detail::Node< T >{v, nullptr, nullptr};
  new_node->prev_ = tail_;
  if (tail_)
  {
    tail_->next_ = new_node;
  }
  else
  {
    head_ = new_node;
  }
  tail_ = new_node;
  ++size_;
}

template < class T >
zharov::LIter< T > zharov::List< T >::insert(LIter< T > pos, const T& v)
{
  if (!pos.curr_)
  {
    pushBack(v);
    return LIter< T >(tail_);
  }
  else if (pos.curr_ == head_)
  {
    pushFront(v);
    return LIter< T >(head_);
  }
  detail::Node< T >* new_node = new detail::Node< T >{v, nullptr, nullptr};
  detail::Node< T >* next = pos.curr_;
  detail::Node< T >* prev = next->prev_;
  new_node->next_ = next;
  new_node->prev_ = prev;
  prev->next_ = new_node;
  next->prev_ = new_node;
  ++size_;
  return LIter< T >(new_node);
}

template < class T >
void zharov::List< T >::popFront()
{
  if (!head_)
  {
    return;
  }
  detail::Node< T >* next = head_->next_;
  if (next)
  {
    next->prev_ = nullptr;
  }
  else
  {
    tail_ = nullptr;
  }
  delete head_;
  head_ = next;
  --size_;
}

template < class T >
void zharov::List< T >::popBack()
{
  if (!tail_)
  {
    return;
  }
  detail::Node< T >* prev = tail_->prev_;
  if (prev)
  {
    prev->next_ = nullptr;
  }
  else
  {
    head_ = nullptr;
  }
  delete tail_;
  tail_ = prev;
  --size_;
}

template < class T >
zharov::LIter< T > zharov::List< T >::erase(LIter< T > pos)
{
  if (!pos.curr_)
  {
    return end();
  }
  detail::Node< T >* next = pos.curr_->next_;
  detail::Node< T >* prev = pos.curr_->prev_;
  if (next)
  {
    next->prev_ = prev;
  }
  else
  {
    tail_ = prev;
  }
  if (prev)
  {
    prev->next_ = next;
  }
  else
  {
    head_ = next;
  }
  delete pos.curr_;
  --size_;
  return LIter< T >(next);
}

template < class T >
void zharov::List< T >::clear()
{
  while (head_)
  {
    detail::Node< T >* next = head_->next_;
    delete head_;
    head_ = next;
    --size_;
  }
  tail_ = nullptr;
}

template < class T >
size_t zharov::List< T >::size() const
{
  return size_;
}

#endif
