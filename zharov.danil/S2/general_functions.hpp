#ifndef GENERAL_FUNCTIONS_HPP
#define GENERAL_FUNCTIONS_HPP
#include <iostream>
#include <queue.hpp>
#include <stack.hpp>

namespace zharov
{
  Queue< std::string > getQueue(const std::string& line);
  Queue< std::string > getPostfix(Queue< std::string >& infix);
  long long calculate(Queue< std::string >& postfix);

  struct Expression
  {
    Expression(const std::string& line);
    long long calculate();

  private:
    Queue< std::string > postfix_;
  };
}

#endif
