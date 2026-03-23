#ifndef STACK_HPP
#define STACK_HPP
#include <list.hpp>

namespace zharov {
  template< class T >
  class Stack {
    List< T > list_;
  public:
    Stack();
    Stack(const Stack & q);
    Stack(Stack && q) noexcept;
    ~Stack() = default;
    Stack & operator=(const Stack & h);
    Stack & operator=(Stack && h) noexcept;
    T & top();
    const T & top() const;
    bool empty() const;
    size_t size() const;
    void clear();
    void push(T rhs);
    T drop();
  };
}

#endif
