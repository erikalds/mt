#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "digg/event_listener.h"
#include "digg/subwindow.h"

class Instrument;

class Keyboard : public digg::SubWindow,
                 public digg::EventListener
{
public:
  Keyboard();

  void draw_widgets() override;
  void event_occurred(const sf::Event& e) override;

  void set_current_instrument(Instrument* instr);

private:
  Instrument* current_instrument = nullptr;
};

#endif /* KEYBOARD_H */
