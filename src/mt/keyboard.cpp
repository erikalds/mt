#include "keyboard.h"

#include "instrument.h"
#include "note.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>


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

  std::function<void(std::size_t, Note)> action;
  if (e.type == sf::Event::KeyReleased)
  {
    action = [this](std::size_t octave, Note note) { current_instrument->stop(octave, note); };
  }
  else
  {
    action = [this](std::size_t octave, Note note) { current_instrument->play(octave, note); };
  }

  auto octave = 3u;
  switch (e.key.code)
  {
  case sf::Keyboard::Z: return action(octave, Note::C);
  case sf::Keyboard::S: return action(octave, Note::C_sharp);
  case sf::Keyboard::X: return action(octave, Note::D);
  case sf::Keyboard::D: return action(octave, Note::D_sharp);
  case sf::Keyboard::C: return action(octave, Note::E);
  case sf::Keyboard::V: return action(octave, Note::F);
  case sf::Keyboard::G: return action(octave, Note::F_sharp);
  case sf::Keyboard::B: return action(octave, Note::G);
  case sf::Keyboard::H: return action(octave, Note::G_sharp);
  case sf::Keyboard::N: return action(octave, Note::A);
  case sf::Keyboard::J: return action(octave, Note::A_sharp);
  case sf::Keyboard::M: return action(octave, Note::B);
  case sf::Keyboard::Comma: return action(octave + 1, Note::C);
  case sf::Keyboard::L: return action(octave + 1, Note::C_sharp);
  case sf::Keyboard::Period: return action(octave + 1, Note::D);
  case sf::Keyboard::Q: return action(octave + 1, Note::C);
  case sf::Keyboard::Num2: return action(octave + 1, Note::C_sharp);
  case sf::Keyboard::W: return action(octave + 1, Note::D);
  case sf::Keyboard::Num3: return action(octave + 1, Note::D_sharp);
  case sf::Keyboard::E: return action(octave + 1, Note::E);
  case sf::Keyboard::R: return action(octave + 1, Note::F);
  case sf::Keyboard::Num5: return action(octave + 1, Note::F_sharp);
  case sf::Keyboard::T: return action(octave + 1, Note::G);
  case sf::Keyboard::Num6: return action(octave + 1, Note::G_sharp);
  case sf::Keyboard::Y: return action(octave + 1, Note::A);
  case sf::Keyboard::Num7: return action(octave + 1, Note::A_sharp);
  case sf::Keyboard::U: return action(octave + 1, Note::B);
  case sf::Keyboard::I: return action(octave + 2, Note::C);
  case sf::Keyboard::Num9: return action(octave + 2, Note::C_sharp);
  case sf::Keyboard::O: return action(octave + 2, Note::D);
  case sf::Keyboard::Num0: return action(octave + 2, Note::D_sharp);
  case sf::Keyboard::P: return action(octave + 2, Note::E);
  default: return;
  }
}

void Keyboard::set_current_instrument(Instrument* instr)
{
  if (current_instrument != nullptr)
  {
    current_instrument->stop();
  }

  current_instrument = instr;
}
