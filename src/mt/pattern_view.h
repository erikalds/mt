#ifndef PATTERN_VIEW_H
#define PATTERN_VIEW_H

#include "digg/subwindow.h"
#include "digg/event_listener.h"

namespace mt { class Pattern; }

class PatternView : public digg::SubWindow,
                    public digg::EventListener

{
public:
  PatternView();

  void display_pattern(mt::Pattern& p);

  void event_occurred(const sf::Event& e) override;

  void draw_widgets() override;

private:
  void render_column_header(std::size_t length);
  void render_row_header(std::size_t length);
  void render_column(const std::vector<std::string>& data);

  mt::Pattern* current_pattern = nullptr;
  std::vector<float> column_widths;
  std::vector<std::string> header_strings;
  std::vector<const char*> headers;
  std::size_t current_column = 0;
  std::size_t current_row = 0;

  std::size_t column;
};

#endif /* PATTERN_VIEW_H */
