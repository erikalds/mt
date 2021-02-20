#ifndef MENU_H
#define MENU_H

#include "menuitem.h"
#include <string>
#include <vector>

namespace digg
{

  class Menu
  {
  public:
    explicit Menu(std::string name);
    Menu(std::string name, std::vector<MenuItem> items);
    Menu(const Menu&);
    Menu& operator=(const Menu&);
    Menu(Menu&&) noexcept;
    Menu& operator=(Menu&&) noexcept;
    ~Menu();

    void add_menuitem(MenuItem item);

    void process();

  private:
    std::string name;
    std::vector<MenuItem> items;
  };

}  // namespace digg

#endif /* MENU_H */
