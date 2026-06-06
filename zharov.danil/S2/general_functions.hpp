#ifndef GENERAL_FUNCTIONS_HPP
#define GENERAL_FUNCTIONS_HPP
#include <iostream>
#include <queue.hpp>
#include <stack.hpp>

namespace zharov
{
  bool isOperand(const std::string& str);
  Queue< std::string > getQueue(const std::string& line);
  Queue< std::string > getPostfix(Queue< std::string >& infix);
  long long calculate(Queue< std::string >& postfix);
  int getPriority(const std::string& op);
  std::istream& getResults(std::istream& in, Stack< long long >& results);
}

#endif
