#ifndef STACK_HPP
#define STACK_HPP
#include <list.hpp>

namespace zharov {
  template< class T >
  class Stack {
  public:
    void push(T rhs);
    T drop();
  };
}

#endif
