#include <iostream>
#include <string>
#include "general_functions.hpp"

using ll_t = long long;

int main()
{
  std::string line = "2 + 2 * 2";
  zharov::Stack< ll_t > results;
  while (!std::getline(std::cin, line).eof()) {
    try {
      zharov::Queue< std::string> queue = zharov::getQueue(line);
      queue = zharov::getPostfix(queue);
      ll_t result = zharov::getResult(queue);
      results.push(result);
    } catch (const std::overflow_error & e) {
      std::cerr << e.what() << "\n";
      return 1;
    } catch (const std::logic_error & e) {
      std::cerr << e.what() << "\n";
      return 1;
    }
  }
  while (!results.empty()) {
    std::cout << results.drop() << " ";
  }
  std::cout << "\n";
}