#include <iostream>
#include <limits>
#include "cafe.hpp"
#include "menu_cmd.hpp"

int main(int argc, char** argv)
{
  zharov::CafeSystem cafe;
  cafe.history_file = (argc > 1) ? argv[1] : "history.txt";

  using Cmd = void (*)(std::ostream&, std::istream&, zharov::CafeSystem&);
  using ConstCmd = void (*)(std::ostream&, std::istream&, const zharov::CafeSystem&);
  zharov::RHHashTable< std::string, Cmd, std::hash< std::string >, std::equal_to< std::string > >
    cmds;
  zharov::RHHashTable< std::string, ConstCmd, std::hash< std::string >,
    std::equal_to< std::string > >
    constCmds;

  cmds.add("menu-add", zharov::menuAdd);
  cmds.add("menu-remove", zharov::menuRemove);
  cmds.add("menu-price", zharov::menuPrice);
  cmds.add("menu-time", zharov::menuTime);
  cmds.add("menu-desc", zharov::menuDesc);
  cmds.add("menu-available", zharov::menuAvailable);
  cmds.add("menu-import", zharov::menuImport);
  cmds.add("menu-copy", zharov::menuCopy);
  constCmds.add("menu-show", zharov::menuShow);
  constCmds.add("menu-export", zharov::menuExport);
  constCmds.add("menu-list", zharov::menuList);

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
    catch (const std::exception& e)
    {
      std::cin.clear();
      std::cout << "<INVALID COMMAND>\n";
      auto skip = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(skip, '\n');
    }
  }
}
