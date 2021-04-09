#ifndef PATTERN_VIEW_H
#define PATTERN_VIEW_H

#include "digg/subwindow.h"
#include "digg/event_listener.h"

struct ImVec2;

class Keyboard;

namespace mt { class Pattern; }

class PatternView : public digg::SubWindow,
                    public digg::EventListener

{
public:
  PatternView();

  void display_pattern(mt::Pattern& p);

  void set_keyboard(const Keyboard& k) { keyboard = &k; }
  void event_occurred(const sf::Event& e) override;

private:
  bool set_current_note_event(const sf::Event& e);

public:
  void draw_widgets() override;

private:
  void render_column_header(std::size_t length);
  void render_row_header(std::size_t length);
  void render_column(const std::vector<std::string>& data);
  void draw_cursor(const std::string& elem) const;
  void draw_rect(const ImVec2& pos, const ImVec2& size,
                 std::uint32_t color) const;

  mt::Pattern* current_pattern = nullptr;
  std::vector<float> column_widths;
  std::vector<std::string> header_strings;
  std::vector<const char*> headers;
  int current_column = 0;
  int current_subcolumn = 0;
  int current_row = 0;

  int column;

  bool editing = false;
  const Keyboard* keyboard = nullptr;
};

#endif /* PATTERN_VIEW_H */
