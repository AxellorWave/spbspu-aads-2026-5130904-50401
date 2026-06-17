#include <iostream>
#include <limits>
#include "cafe.hpp"

int main(int argc, char** argv)
{
  zharov::CafeSystem cafe;
  cafe.history_file = (argc > 1) ? argv[1] : "history.txt";

  using Cmd = void (*)(std::ostream&, std::istream&, zharov::CafeSystem&);
  using ConstCmd = void (*)(std::ostream&, std::istream&, const zharov::CafeSystem&);
  zharov::RHHashTable< std::string, Cmd, std::hash< std::string >, std::equal_to< std::string > >
    cmds;
  zharov::RHHashTable< std::string, ConstCmd, std::hash< std::string >, std::equal_to< std::string > >
    constCmds;

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      if (cmds.has(command))
      {
        cmds.at(command)(std::cout, std::cin, cafe);
      }
      else
      {
        constCmds.at(command)(std::cout, std::cin, cafe);
      }
    }
    catch (...)
    {
      std::cin.clear();
      std::cout << "<INVALID COMMAND>\n";
      auto skip = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(skip, '\n');
    }
  }
}
