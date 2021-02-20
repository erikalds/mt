#include "subwindow.h"

#include "menubar.h"
#include "sentry.h"
#include <imgui.h>

namespace digg
{

  SubWindow::SubWindow(std::string name_) :
    name{name_},
    is_open{true}
  {
  }

  SubWindow::~SubWindow() = default;

  void SubWindow::set_menubar(MenuBar mb)
  {
    menubar = std::move(mb);
  }

  void SubWindow::process()
  {
    if (!is_open)
      return;

    ImGuiWindowFlags flags = 0;
    if (menubar)
      flags |= ImGuiWindowFlags_MenuBar;

    bool will_draw = ImGui::Begin(name.c_str(), &is_open, flags);
    Sentry sentry{[]() { ImGui::End(); }};
    if (will_draw)
    {
      if (menubar)
        menubar->process();
    }

    draw_widgets();
  }

}  // namespace digg
