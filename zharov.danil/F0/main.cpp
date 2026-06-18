#include <iostream>
#include <limits>
#include "analyze.hpp"
#include "cafe.hpp"
#include "history.hpp"
#include "menu.hpp"
#include "optimize.hpp"
#include "order.hpp"
#include "queue.hpp"

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

  cmds.add("order-create", zharov::orderCreate);
  cmds.add("order-add", zharov::orderAdd);
  cmds.add("order-remove", zharov::orderRemove);
  cmds.add("order-cancel", zharov::orderCancel);
  constCmds.add("order-show", zharov::orderShow);

  cmds.add("queue-create", zharov::queueCreate);
  cmds.add("queue-remove", zharov::queueRemove);
  cmds.add("queue-complete", zharov::queueComplete);
  cmds.add("queue-reject", zharov::queueReject);
  constCmds.add("queue-show", zharov::queueShow);
  constCmds.add("queue-list", zharov::queueList);

  cmds.add("optimize", zharov::cmdOptimize);

  constCmds.add("history", zharov::cmdHistory);

  constCmds.add("analyze-item", zharov::analyzeItem);

  std::string command;
  while (std::cin >> command)
  {
    if (!cmds.has(command) && !constCmds.has(command))
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      continue;
    }
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
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
