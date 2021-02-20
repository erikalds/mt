#include "menuitem.h"

#include "action.h"
#include <imgui.h>

namespace digg
{

  MenuItem::MenuItem(Action& action_) :
    action{&action_}
  {
  }

  void MenuItem::process()
  {
    auto optional_shortcut = action->shortcut_text();
    if (optional_shortcut)
    {
      if (ImGui::MenuItem(action->name().data(), optional_shortcut->data()))
      {
        action->act();
      }
    }
    else
    {
      if (ImGui::MenuItem(action->name().data()))
      {
        action->act();
      }
    }
  }

}  // namespace digg
