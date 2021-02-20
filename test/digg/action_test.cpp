#include "digg/action.h"
#include <catch2/catch.hpp>

TEST_CASE("action.name", "[digg]")
{
  CHECK(digg::Action{"name", [](){}}.name() == "name");
  CHECK(digg::Action{"othername", [](){}}.name() == "othername");
}

TEST_CASE("action.shortcut text", "[digg]")
{
  digg::Action a{"name", [](){}};
  a.set_shortcut(sf::Keyboard::K, true, false);
  CHECK(a.shortcut_text() == "Ctrl+K");
  a.set_shortcut(sf::Keyboard::X, false, true);
  CHECK(a.shortcut_text() == "Alt+X");
  a.set_shortcut(sf::Keyboard::W, true, true);
  CHECK(a.shortcut_text() == "Ctrl+Alt+W");
  a.set_shortcut(sf::Keyboard::V, false, false);
  CHECK(a.shortcut_text() == "V");
}
