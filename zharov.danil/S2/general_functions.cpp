#include "general_functions.hpp"
#include <stdexcept>
#include "math_functions.hpp"

namespace
{
  bool isOperator(const std::string& str)
  {
    std::string signs[] = {"+", "-", "/", "*", "%", "<<", "(", ")"};
    for (size_t i = 0; i < 8; ++i)
    {
      if (str == signs[i])
      {
        return true;
      }
    }
    return false;
  }

  zharov::Queue< std::string > getQueue(const std::string& line)
  {
    zharov::Queue< std::string > res;
    std::string curr;
    for (size_t i = 0; line[i] != '\0'; ++i)
    {
      if (line[i] == ' ')
      {
        res.push(curr);
        curr.clear();
      }
      else
      {
        curr.push_back(line[i]);
      }
    }
    if (!curr.empty())
    {
      res.push(curr);
    }
    return res;
  }

  int getPriority(const std::string& op)
  {
    if (op == "<<")
    {
      return 3;
    }
    if (op == "*" || op == "/")
    {
      return 1;
    }
    return 2;
  }

  zharov::Queue< std::string > getPostfix(zharov::Queue< std::string >& infix)
  {
    zharov::Stack< std::string > stack;
    zharov::Queue< std::string > res;
    while (!infix.empty())
    {
      std::string curr = infix.front();
      infix.pop();
      if (!isOperator(curr))
      {
        res.push(curr);
      }
      else if (curr == "(")
      {
        stack.push(curr);
      }
      else if (curr == ")")
      {
        while (!stack.empty() && stack.top() != "(")
        {
          res.push(stack.top());
          stack.pop();
        }
        if (!stack.empty())
        {
          stack.pop();
        }
        else
        {
          throw std::logic_error("Bad sign (");
        }
      }
      else
      {
        while (!stack.empty() && stack.top() != "(")
        {
          if (getPriority(curr) >= getPriority(stack.top()))
          {
            res.push(stack.top());
            stack.pop();
          }
          else
          {
            break;
          }
        }
        stack.push(curr);
      }
    }
    while (!stack.empty())
    {
      if (stack.top() != "(")
      {
        res.push(stack.top());
        stack.pop();
      }
      else
      {
        throw std::logic_error("Bad sign (");
      }
    }
    return res;
  }

  zharov::ll_t calculate(zharov::Queue< std::string >& postfix)
  {
    zharov::Stack< zharov::ll_t > temp;
    while (!postfix.empty())
    {
      std::string curr = postfix.front();
      postfix.pop();
      if (isOperator(curr))
      {
        if (temp.size() < 2)
        {
          throw std::logic_error("Not enough operands for operator: " + curr);
        }
        else
        {
          zharov::ll_t b = temp.top();
          temp.pop();
          zharov::ll_t a = temp.top();
          temp.pop();
          if (curr == "+")
          {
            temp.push(zharov::add(a, b));
          }
          else if (curr == "-")
          {
            temp.push(zharov::sub(a, b));
          }
          else if (curr == "*")
          {
            temp.push(zharov::mul(a, b));
          }
          else if (curr == "/")
          {
            temp.push(zharov::div(a, b));
          }
          else if (curr == "%")
          {
            temp.push(zharov::mod(a, b));
          }
          else if (curr == "<<")
          {
            temp.push(zharov::bitShiftLeft(a, b));
          }
          else
          {
            throw std::logic_error("Unknown operator" + curr);
          }
        }
      }
      else
      {
        temp.push(std::stoll(curr));
      }
    }
    if (temp.size() != 1)
    {
      throw std::logic_error("Not enough operators");
    }
    zharov::ll_t result = temp.top();
    temp.pop();
    return result;
  }
}

std::istream& zharov::getResults(std::istream& in, Stack< ll_t >& results)
{
  std::string line;
  while (!std::getline(in, line).eof())
  {
    if (line.empty())
    {
      continue;
    }
    Queue< std::string > queue = getQueue(line);
    queue = getPostfix(queue);
    ll_t result = calculate(queue);
    results.push(result);
  }
  return in;
}
