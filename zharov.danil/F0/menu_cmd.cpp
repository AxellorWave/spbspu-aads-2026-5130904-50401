#include "menu_cmd.hpp"
#include <iomanip>
#include <stdexcept>
#include <string>

namespace
{
  bool hasMoreArgs(std::istream& in)
  {
    while (in.peek() == ' ')
    {
      in.get();
    }
    return in.peek() != '\n' && in.peek() != EOF;
  }
}

void zharov::menuAdd(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string menuName, name, description;
  int price = 0, prepTime = 0;
  in >> menuName >> name >> price >> prepTime;
  if (hasMoreArgs(in))
  {
    in >> std::quoted(description);
  }

  if (price <= 0)
  {
    throw std::invalid_argument("Price must be positive");
  }
  if (prepTime <= 0)
  {
    throw std::invalid_argument("Prep time must be positive");
  }
  if (!cafe.menus.has(menuName))
  {
    cafe.menus.add(menuName, zharov::MenuTable{});
  }
  zharov::MenuTable& menu = cafe.menus.at(menuName);
  if (menu.has(name))
  {
    throw std::invalid_argument("Item already exists");
  }
  menu.add(name, zharov::MenuItem{price, prepTime, description, true});
  out << "<OK: Added item \"" << name << "\">\n";
}
