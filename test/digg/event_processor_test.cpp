#include "digg/action.h"
#include "digg/event_processor.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("process shortcuts", "[digg]")
{
  digg::EventProcessor proc{};
  int ctrl_a_called = 0;
  digg::Action action_ctrl_a{"An action", [&]() { ++ctrl_a_called; }};
  action_ctrl_a.set_shortcut(sf::Keyboard::A, true, false);
  proc.add_action(action_ctrl_a);
  int a_called = 0;
  digg::Action action_a{"Another action", [&]() { ++a_called; }};
  action_a.set_shortcut(sf::Keyboard::A, false, false);
  proc.add_action(action_a);

  sf::Event e{};
  e.type = sf::Event::KeyPressed;
  e.key.control = false;
  e.key.alt = false;
  e.key.shift = false;
  e.key.system = false;
  e.key.code = sf::Keyboard::A;
  proc.process_event(e);

  CHECK(a_called == 1);
  CHECK(ctrl_a_called == 0);

  e.key.control = true;
  proc.process_event(e);
  CHECK(a_called == 1);
  CHECK(ctrl_a_called == 1);

  e.type = sf::Event::MouseMoved;
  proc.process_event(e);
  CHECK(a_called == 1);
  CHECK(ctrl_a_called == 1);
}
