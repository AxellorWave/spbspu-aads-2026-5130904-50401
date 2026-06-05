#ifndef STACK_HPP
#define STACK_HPP
#include "list.hpp"

namespace zharov
{
  template < class T >
  class Stack
  {
  public:
    T& top();
    const T& top() const;
    bool empty() const;
    size_t size() const;
    void clear();
    void push(T rhs);
    T drop();
    void swap(Stack& s);

  private:
    List< T > list_;
  };
}

template < class T >
T& zharov::Stack< T >::top()
{
  return list_.back();
}

template < class T >
const T& zharov::Stack< T >::top() const
{
  return list_.back();
}

template < class T >
bool zharov::Stack< T >::empty() const
{
  return !list_.size();
}

template < class T >
size_t zharov::Stack< T >::size() const
{
  return list_.size();
}

template < class T >
void zharov::Stack< T >::clear()
{
  list_.clear();
}

template < class T >
void zharov::Stack< T >::push(T rhs)
{
  list_.pushBack(rhs);
}

template < class T >
T zharov::Stack< T >::drop()
{
  T tp = top();
  list_.popBack();
  return tp;
}

template < class T >
void zharov::Stack< T >::swap(Stack& s)
{
  list_.swap(s.list_);
}

#endif
