#include "keyboard.h"

#include "mtlib/instrument.h"
#include "mtlib/note.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cassert>


Keyboard::Keyboard() :
  digg::SubWindow{"Keyboard"}
{
}

void Keyboard::draw_widgets()
{
}

void Keyboard::event_occurred(const sf::Event& e)
{
  if (!is_active()
      || !(e.type == sf::Event::KeyPressed
           || e.type == sf::Event::KeyReleased)
      || current_instrument == nullptr
      || (e.key.alt || e.key.control || e.key.shift || e.key.system))
  {
    return;
  }

  auto opt_notedef = get_note_from_event(e);
  if (!opt_notedef)
  {
    return;
  }

  if (e.type == sf::Event::KeyReleased)
  {
    current_instrument->stop(*opt_notedef);
  }
  else
  {
    current_instrument->play(*opt_notedef);
  }
}

[[nodiscard]] std::optional<mt::NoteDef>
Keyboard::get_note_from_event(const sf::Event& e) const
{
  assert(e.type == sf::Event::KeyPressed || e.type == sf::Event::KeyReleased);
  auto octave = 3u;
  switch (e.key.code)
  {
  case sf::Keyboard::Z: return mt::NoteDef{mt::Note::C, octave};
  case sf::Keyboard::S: return mt::NoteDef{mt::Note::C_sharp, octave};
  case sf::Keyboard::X: return mt::NoteDef{mt::Note::D, octave};
  case sf::Keyboard::D: return mt::NoteDef{mt::Note::D_sharp, octave};
  case sf::Keyboard::C: return mt::NoteDef{mt::Note::E, octave};
  case sf::Keyboard::V: return mt::NoteDef{mt::Note::F, octave};
  case sf::Keyboard::G: return mt::NoteDef{mt::Note::F_sharp, octave};
  case sf::Keyboard::B: return mt::NoteDef{mt::Note::G, octave};
  case sf::Keyboard::H: return mt::NoteDef{mt::Note::G_sharp, octave};
  case sf::Keyboard::N: return mt::NoteDef{mt::Note::A, octave};
  case sf::Keyboard::J: return mt::NoteDef{mt::Note::A_sharp, octave};
  case sf::Keyboard::M: return mt::NoteDef{mt::Note::B, octave};
  case sf::Keyboard::Comma: return mt::NoteDef{mt::Note::C, octave + 1};
  case sf::Keyboard::L: return mt::NoteDef{mt::Note::C_sharp, octave + 1};
  case sf::Keyboard::Period: return mt::NoteDef{mt::Note::D, octave + 1};
  case sf::Keyboard::Q: return mt::NoteDef{mt::Note::C, octave + 1};
  case sf::Keyboard::Num2: return mt::NoteDef{mt::Note::C_sharp, octave + 1};
  case sf::Keyboard::W: return mt::NoteDef{mt::Note::D, octave + 1};
  case sf::Keyboard::Num3: return mt::NoteDef{mt::Note::D_sharp, octave + 1};
  case sf::Keyboard::E: return mt::NoteDef{mt::Note::E, octave + 1};
  case sf::Keyboard::R: return mt::NoteDef{mt::Note::F, octave + 1};
  case sf::Keyboard::Num5: return mt::NoteDef{mt::Note::F_sharp, octave + 1};
  case sf::Keyboard::T: return mt::NoteDef{mt::Note::G, octave + 1};
  case sf::Keyboard::Num6: return mt::NoteDef{mt::Note::G_sharp, octave + 1};
  case sf::Keyboard::Y: return mt::NoteDef{mt::Note::A, octave + 1};
  case sf::Keyboard::Num7: return mt::NoteDef{mt::Note::A_sharp, octave + 1};
  case sf::Keyboard::U: return mt::NoteDef{mt::Note::B, octave + 1};
  case sf::Keyboard::I: return mt::NoteDef{mt::Note::C, octave + 2};
  case sf::Keyboard::Num9: return mt::NoteDef{mt::Note::C_sharp, octave + 2};
  case sf::Keyboard::O: return mt::NoteDef{mt::Note::D, octave + 2};
  case sf::Keyboard::Num0: return mt::NoteDef{mt::Note::D_sharp, octave + 2};
  case sf::Keyboard::P: return mt::NoteDef{mt::Note::E, octave + 2};
  default: return std::optional<mt::NoteDef>{};
  }
}

void Keyboard::set_current_instrument(mt::Instrument* instr)
{
  if (current_instrument != nullptr)
  {
    current_instrument->stop();
  }

  current_instrument = instr;
}
