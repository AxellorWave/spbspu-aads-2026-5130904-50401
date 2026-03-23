#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <list.hpp>

namespace zharov {
  template< class T >
  class Queue {
  public:
    void push(T rhs);
    T drop();
  };
}

#endif
