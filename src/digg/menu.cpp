#include "menu.h"

#include "menuitem.h"
#include "sentry.h"
#include <imgui.h>

namespace digg
{

  Menu::Menu(std::string name_) :
    Menu{name_, std::vector<MenuItem>{}}
  {
  }

  Menu::Menu(std::string name_, std::vector<MenuItem> items_) :
    name{name_},
    items{std::move(items_)}
  {
  }

  Menu::~Menu() = default;

  void Menu::add_menuitem(MenuItem item)
  {
    items.push_back(std::move(item));
  }

  void Menu::process()
  {
    if (ImGui::BeginMenu(name.c_str()))
    {
      Sentry sentry{[](){ ImGui::EndMenu(); }};
      for (auto& item : items)
        item.process();
    }
  }

}  // namespace digg
