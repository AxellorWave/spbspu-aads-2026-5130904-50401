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
    T& operator*() noexcept;
    T* operator->() noexcept;
    LIter& operator++() noexcept;
    LIter operator++(int) noexcept;
    LIter& operator--() noexcept;
    LIter operator--(int) noexcept;
    bool operator==(const LIter& it) const noexcept;
    bool operator!=(const LIter& it) const noexcept;

  private:
    detail::Node< T >* curr_;
    LIter(detail::Node< T >* node) noexcept;
  };

  template < class T >
  class LCIter
  {
    friend class List< T >;

  public:
    const T& operator*() const noexcept;
    const T* operator->() const noexcept;
    LCIter& operator++() noexcept;
    LCIter operator++(int) noexcept;
    LCIter& operator--() noexcept;
    LCIter operator--(int) noexcept;
    bool operator==(const LCIter& it) const noexcept;
    bool operator!=(const LCIter& it) const noexcept;

  private:
    const detail::Node< T >* curr_;
    LCIter(const detail::Node< T >* node) noexcept;
  };

  template < class T >
  class List
  {
  public:
    List() noexcept;
    List(const List& h);
    List(List&& h) noexcept;
    ~List() noexcept;
    List& operator=(const List& h);
    List& operator=(List&& h) noexcept;
    LIter< T > begin() noexcept;
    LIter< T > end() noexcept;
    LCIter< T > cbegin() const noexcept;
    LCIter< T > cend() const noexcept;
    T& front() noexcept;
    const T& front() const noexcept;
    T& back() noexcept;
    const T& back() const noexcept;
    void pushFront(const T& v);
    void pushBack(const T& v);
    LIter< T > insert(LIter< T > pos, const T& v);
    void pushFront(T&& v);
    void pushBack(T&& v);
    LIter< T > insert(LIter< T > pos, T&& v);
    void popFront() noexcept;
    void popBack() noexcept;
    LIter< T > erase(LIter< T > pos) noexcept;
    void clear() noexcept;
    size_t size() const noexcept;
    void swap(List< T >& h) noexcept;
    void splice(LIter< T > pos, List< T >& other) noexcept;
    void splice(LIter< T > pos, List< T >& other, LIter< T > it) noexcept;
    void splice(LIter< T > pos, List< T >& other, LIter< T > first, LIter< T > last) noexcept;

  private:
    template < class U >
    void pushFrontImpl(U&& v);
    template < class U >
    void pushBackImpl(U&& v);
    template < class U >
    LIter< T > insertImpl(LIter< T > pos, U&& v);
    void spliceRange(detail::Node< T >* first, detail::Node< T >* last,
      size_t count, List< T >& src, detail::Node< T >* pos) noexcept;
    detail::Node< T >* head_;
    detail::Node< T >* tail_;
    size_t size_;
  };
}

template < class T >
zharov::LIter< T >::LIter(detail::Node< T >* node) noexcept:
  curr_(node)
{}

template < class T >
T& zharov::LIter< T >::operator*() noexcept
{
  return curr_->val_;
}

template < class T >
T* zharov::LIter< T >::operator->() noexcept
{
  return std::addressof(curr_->val_);
}

template < class T >
zharov::LIter< T >& zharov::LIter< T >::operator++() noexcept
{
  curr_ = curr_->next_;
  return *this;
}

template < class T >
zharov::LIter< T > zharov::LIter< T >::operator++(int) noexcept
{
  LIter< T > temp = *this;
  curr_ = curr_->next_;
  return temp;
}

template < class T >
zharov::LIter< T >& zharov::LIter< T >::operator--() noexcept
{
  curr_ = curr_->prev_;
  return *this;
}

template < class T >
zharov::LIter< T > zharov::LIter< T >::operator--(int) noexcept
{
  LIter< T > temp = *this;
  curr_ = curr_->prev_;
  return temp;
}

template < class T >
bool zharov::LIter< T >::operator==(const LIter& it) const noexcept
{
  return curr_ == it.curr_;
}

template < class T >
bool zharov::LIter< T >::operator!=(const LIter& it) const noexcept
{
  return curr_ != it.curr_;
}

template < class T >
zharov::LCIter< T >::LCIter(const detail::Node< T >* node) noexcept:
  curr_(node)
{}

template < class T >
const T& zharov::LCIter< T >::operator*() const noexcept
{
  return curr_->val_;
}

template < class T >
const T* zharov::LCIter< T >::operator->() const noexcept
{
  return std::addressof(curr_->val_);
}

template < class T >
zharov::LCIter< T >& zharov::LCIter< T >::operator++() noexcept
{
  curr_ = curr_->next_;
  return *this;
}

template < class T >
zharov::LCIter< T > zharov::LCIter< T >::operator++(int) noexcept
{
  LCIter< T > temp = *this;
  curr_ = curr_->next_;
  return temp;
}

template < class T >
zharov::LCIter< T >& zharov::LCIter< T >::operator--() noexcept
{
  curr_ = curr_->prev_;
  return *this;
}

template < class T >
zharov::LCIter< T > zharov::LCIter< T >::operator--(int) noexcept
{
  LCIter< T > temp = *this;
  curr_ = curr_->prev_;
  return temp;
}

template < class T >
bool zharov::LCIter< T >::operator==(const LCIter& it) const noexcept
{
  return curr_ == it.curr_;
}

template < class T >
bool zharov::LCIter< T >::operator!=(const LCIter& it) const noexcept
{
  return curr_ != it.curr_;
}

template < class T >
zharov::List< T >::List() noexcept:
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
  head_(std::exchange(h.head_, nullptr)),
  tail_(std::exchange(h.tail_, nullptr)),
  size_(std::exchange(h.size_, 0))
{}

template < class T >
zharov::List< T >::~List() noexcept
{
  clear();
}

template < class T >
void zharov::List< T >::swap(List< T >& h) noexcept
{
  std::swap(h.head_, head_);
  std::swap(h.tail_, tail_);
  std::swap(h.size_, size_);
}

template < class T >
zharov::List< T >& zharov::List< T >::operator=(const List< T >& h)
{
  if (this != std::addressof(h))
  {
    List< T > temp(h);
    swap(temp);
  }
  return *this;
}

template < class T >
zharov::List< T >& zharov::List< T >::operator=(List< T >&& h) noexcept
{
  if (this != std::addressof(h))
  {
    List tmp(std::move(h));
    swap(tmp);
  }
  return *this;
}

template < class T >
zharov::LIter< T > zharov::List< T >::begin() noexcept
{
  return LIter< T >(head_);
}

template < class T >
zharov::LIter< T > zharov::List< T >::end() noexcept
{
  return LIter< T >(nullptr);
}

template < class T >
zharov::LCIter< T > zharov::List< T >::cbegin() const noexcept
{
  return LCIter< T >(head_);
}

template < class T >
zharov::LCIter< T > zharov::List< T >::cend() const noexcept
{
  return LCIter< T >(nullptr);
}

template < class T >
T& zharov::List< T >::front() noexcept
{
  return head_->val_;
}

template < class T >
const T& zharov::List< T >::front() const noexcept
{
  return head_->val_;
}

template < class T >
T& zharov::List< T >::back() noexcept
{
  return tail_->val_;
}

template < class T >
const T& zharov::List< T >::back() const noexcept
{
  return tail_->val_;
}

template < class T >
template < class U >
void zharov::List< T >::pushFrontImpl(U&& v)
{
  detail::Node< T >* new_node = new detail::Node< T >{std::forward< U >(v), nullptr, nullptr};
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
template < class U >
void zharov::List< T >::pushBackImpl(U&& v)
{
  detail::Node< T >* new_node = new detail::Node< T >{std::forward< U >(v), nullptr, nullptr};
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
template < class U >
zharov::LIter< T > zharov::List< T >::insertImpl(LIter< T > pos, U&& v)
{
  if (!pos.curr_)
  {
    pushBackImpl(std::forward< U >(v));
    return LIter< T >(tail_);
  }
  if (pos.curr_ == head_)
  {
    pushFrontImpl(std::forward< U >(v));
    return LIter< T >(head_);
  }
  detail::Node< T >* new_node = new detail::Node< T >{std::forward< U >(v), nullptr, nullptr};
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
void zharov::List< T >::pushFront(const T& v)
{
  pushFrontImpl(v);
}

template < class T >
void zharov::List< T >::pushFront(T&& v)
{
  pushFrontImpl(std::move(v));
}

template < class T >
void zharov::List< T >::pushBack(const T& v)
{
  pushBackImpl(v);
}

template < class T >
void zharov::List< T >::pushBack(T&& v)
{
  pushBackImpl(std::move(v));
}

template < class T >
zharov::LIter< T > zharov::List< T >::insert(LIter< T > pos, const T& v)
{
  return insertImpl(pos, v);
}

template < class T >
zharov::LIter< T > zharov::List< T >::insert(LIter< T > pos, T&& v)
{
  return insertImpl(pos, std::move(v));
}

template < class T >
void zharov::List< T >::popFront() noexcept
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
void zharov::List< T >::popBack() noexcept
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
zharov::LIter< T > zharov::List< T >::erase(LIter< T > pos) noexcept
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
void zharov::List< T >::clear() noexcept
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
size_t zharov::List< T >::size() const noexcept
{
  return size_;
}

template < class T >
void zharov::List< T >::spliceRange(detail::Node< T >* first, detail::Node< T >* last,
  size_t count, List< T >& src, detail::Node< T >* pos) noexcept
{
  detail::Node< T >* before_first = first->prev_;
  detail::Node< T >* after_last = last->next_;
  if (before_first)
  {
    before_first->next_ = after_last;
  }
  else
  {
    src.head_ = after_last;
  }
  if (after_last)
  {
    after_last->prev_ = before_first;
  }
  else
  {
    src.tail_ = before_first;
  }
  src.size_ -= count;
  first->prev_ = (pos != nullptr) ? pos->prev_ : tail_;
  last->next_ = pos;
  if (first->prev_)
  {
    first->prev_->next_ = first;
  }
  else
  {
    head_ = first;
  }
  if (last->next_)
  {
    last->next_->prev_ = last;
  }
  else
  {
    tail_ = last;
  }
  size_ += count;
}

template < class T >
void zharov::List< T >::splice(LIter< T > pos, List< T >& other) noexcept
{
  if (!other.head_)
  {
    return;
  }
  spliceRange(other.head_, other.tail_, other.size_, other, pos.curr_);
}

template < class T >
void zharov::List< T >::splice(LIter< T > pos, List< T >& other, LIter< T > it) noexcept
{
  spliceRange(it.curr_, it.curr_, 1, other, pos.curr_);
}

template < class T >
void zharov::List< T >::splice(LIter< T > pos, List< T >& other, LIter< T > first, LIter< T > last) noexcept
{
  if (first.curr_ == last.curr_)
  {
    return;
  }
  detail::Node< T >* last_node = (last.curr_ != nullptr) ? last.curr_->prev_ : other.tail_;
  size_t count = 0;
  for (detail::Node< T >* curr = first.curr_; curr != last.curr_; curr = curr->next_)
  {
    ++count;
  }
  spliceRange(first.curr_, last_node, count, other, pos.curr_);
}

#endif
