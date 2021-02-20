#include "mainmenubar.h"

#include "menu.h"
#include "sentry.h"
#include <imgui.h>

namespace digg
{
  MainMenuBar::MainMenuBar() = default;
  MainMenuBar::MainMenuBar(const MainMenuBar&) = default;
  MainMenuBar& MainMenuBar::operator=(const MainMenuBar&) = default;
  MainMenuBar::MainMenuBar(MainMenuBar&&) noexcept = default;
  MainMenuBar& MainMenuBar::operator=(MainMenuBar&&) noexcept = default;
  MainMenuBar::~MainMenuBar() = default;

  void MainMenuBar::process()
  {
    if (menus.empty())
    {
      return;
    }

    if (ImGui::BeginMainMenuBar())
    {
      Sentry sentry{[]() { ImGui::EndMainMenuBar(); }};
      for (auto& m : menus)
      {
        m.process();
      }
    }
  }

  void MainMenuBar::add_menu(const Menu& menu)
  {
    menus.push_back(menu);
  }

}  // digg
