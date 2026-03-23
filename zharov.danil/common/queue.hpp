#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <list.hpp>

namespace zharov {
  template< class T >
  class Queue {
    List< T > list_;
  public:
    Queue();
    Queue(const Queue & q);
    Queue(Queue && q) noexcept;
    ~Queue() = default;
    Queue & operator=(const Queue & h);
    Queue & operator=(Queue && h) noexcept;
    T & front();
    const T & front() const;
    T & back();
    const T & back() const;
    bool empty() const;
    size_t size() const;
    void clear();
    void push(T rhs);
    T drop();
  };
}

#endif
