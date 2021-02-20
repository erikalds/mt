#ifndef MAINMENUBAR_H
#define MAINMENUBAR_H

#include "widget.h"
#include <vector>

namespace digg {

  class Menu;

  class MainMenuBar : public Widget
  {
  public:
    MainMenuBar();
    MainMenuBar(const MainMenuBar&);
    MainMenuBar& operator=(const MainMenuBar&);
    MainMenuBar(MainMenuBar&&);
    MainMenuBar& operator=(MainMenuBar&&);
    ~MainMenuBar();

    void process() override;
    void add_menu(const Menu& menu);

  private:
    std::vector<Menu> menus;
  };

}  // digg

#endif /* MAINMENUBAR_H */
