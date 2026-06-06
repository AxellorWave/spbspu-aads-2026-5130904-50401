#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "general_functions.hpp"

int main(int argc, char** argv)
{
  zharov::Stack< long long > results;
  try
  {
    std::ifstream file;
    std::istream* in = std::addressof(std::cin);
    if (argc > 1)
    {
      file.open(argv[1]);
      in = std::addressof(file);
    }
    std::string line;
    while (!std::getline(*in, line).eof())
    {
      if (line.empty())
      {
        continue;
      }
      zharov::Queue< std::string > queue = zharov::getQueue(line);
      queue = zharov::getPostfix(queue);
      results.push(zharov::calculate(queue));
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  bool is_first = true;
  while (!results.empty())
  {
    if (!is_first)
    {
      std::cout << " ";
    }
    else
    {
      is_first = false;
    }
    std::cout << results.top();
    results.pop();
  }
  std::cout << "\n";
}
