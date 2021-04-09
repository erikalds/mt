#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "instrument_selection_listener.h"
#include "digg/event_listener.h"
#include "digg/subwindow.h"
#include <optional>

namespace mt {
  class Instrument;
  struct NoteDef;
}

class Keyboard : public digg::SubWindow,
                 public digg::EventListener,
                 public InstrumentSelectionListener
{
public:
  Keyboard();

  void draw_widgets() override;
  void event_occurred(const sf::Event& e) override;

  [[nodiscard]] std::optional<mt::NoteDef> get_note_from_event(const sf::Event& e) const;

  void set_current_instrument(mt::Instrument* instr);
  void selected_instrument_changed(mt::Instrument* instr, mt::Sample*) override
  { set_current_instrument(instr); }

private:
  mt::Instrument* current_instrument = nullptr;
};

#endif /* KEYBOARD_H */
