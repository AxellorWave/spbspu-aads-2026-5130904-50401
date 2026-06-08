#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP
#include <iostream>
#include <queue.hpp>
#include <stack.hpp>

namespace zharov
{
  struct Expression
  {
    Expression(const std::string& line);
    long long calculate();

  private:
    Queue< std::string > postfix_;
  };
}

#endif
