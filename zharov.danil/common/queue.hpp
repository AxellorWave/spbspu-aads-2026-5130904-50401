#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "list.hpp"

namespace zharov
{
  template < class T >
  class Queue
  {
  public:
    T& front();
    const T& front() const;
    bool empty() const;
    size_t size() const;
    void clear();
    void push(T rhs);
    T drop();
    void swap(Queue& q);

  private:
    List< T > list_;
  };
}

template < class T >
T& zharov::Queue< T >::front()
{
  return list_.front();
}

template < class T >
const T& zharov::Queue< T >::front() const
{
  return list_.front();
}

template < class T >
bool zharov::Queue< T >::empty() const
{
  return !list_.size();
}

template < class T >
size_t zharov::Queue< T >::size() const
{
  return list_.size();
}

template < class T >
void zharov::Queue< T >::clear()
{
  list_.clear();
}

template < class T >
void zharov::Queue< T >::push(T rhs)
{
  list_.pushBack(rhs);
}

template < class T >
T zharov::Queue< T >::drop()
{
  T fr = front();
  list_.popFront();
  return fr;
}

template < class T >
void zharov::Queue< T >::swap(Queue& q)
{
  list_.swap(q.list_);
}

#endif
