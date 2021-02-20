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
    ~MenuBar();

    void process();
    void add_menu(Menu menu);

  private:
    std::vector<Menu> menus;
};

}  // namespace digg

#endif /* MENUBAR_H */
