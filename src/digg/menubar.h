#ifndef MENUBAR_H
#define MENUBAR_H

#include "menu.h"
#include <vector>

namespace digg
{

  class MenuBar
  {
  public:
    MenuBar();
    explicit MenuBar(std::vector<Menu> menus);
    MenuBar(const MenuBar&);
    MenuBar& operator=(const MenuBar&);
    MenuBar(MenuBar&&) noexcept;
    MenuBar& operator=(MenuBar&&) noexcept;
    ~MenuBar();

    void process();
    void add_menu(const Menu& menu);

  private:
    std::vector<Menu> menus;
};

}  // namespace digg

#endif /* MENUBAR_H */
