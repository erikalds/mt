#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include "menubar.h"
#include "widget.h"
#include <optional>
#include <string>

namespace digg
{

  /// Create a window in ImGui lingo, i.e., a window within the main window.
  class SubWindow : public Widget
  {
  public:
    explicit SubWindow(std::string name_);
    ~SubWindow();

    void set_menubar(MenuBar mb);

    void process();
    void toggle_active() { is_open = !is_open; }

  private:
    virtual void draw_widgets() {}

    std::string name;
    bool is_open;
    std::optional<MenuBar> menubar;
  };

}  // namespace digg

#endif /* SUBWINDOW_H */
