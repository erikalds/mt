#include "menubar.h"

#include "menu.h"
#include "sentry.h"
#include <imgui.h>

namespace digg
{

  MenuBar::MenuBar() :
    MenuBar{std::vector<Menu>{}}
  {
  }

  MenuBar::MenuBar(std::vector<Menu> menus_) :
    menus{std::move(menus_)}
  {
  }

  MenuBar::MenuBar(const MenuBar&) = default;
  MenuBar& MenuBar::operator=(const MenuBar&) = default;
  MenuBar::MenuBar(MenuBar&&) noexcept = default;
  MenuBar& MenuBar::operator=(MenuBar&&) noexcept = default;
  MenuBar::~MenuBar() = default;

  void MenuBar::process()
  {
    if (ImGui::BeginMenuBar())
    {
      Sentry sentry{[]() { ImGui::EndMenuBar(); }};
      for (auto& m : menus)
      {
        m.process();
      }
    }
  }

  void MenuBar::add_menu(const Menu& menu)
  {
    menus.push_back(menu);
  }

}  // namespace digg
