#include "keyboard.h"

#include "mtlib/instrument.h"
#include "mtlib/note.h"

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

  std::function<void(std::size_t, mt::Note)> action;
  if (e.type == sf::Event::KeyReleased)
  {
    action = [this](std::size_t octave, mt::Note note)
             {
               current_instrument->stop(mt::NoteDef{note, octave});
             };
  }
  else
  {
    action = [this](std::size_t octave, mt::Note note)
             {
               current_instrument->play(mt::NoteDef{note, octave});
             };
  }

  auto octave = 3u;
  switch (e.key.code)
  {
  case sf::Keyboard::Z: return action(octave, mt::Note::C);
  case sf::Keyboard::S: return action(octave, mt::Note::C_sharp);
  case sf::Keyboard::X: return action(octave, mt::Note::D);
  case sf::Keyboard::D: return action(octave, mt::Note::D_sharp);
  case sf::Keyboard::C: return action(octave, mt::Note::E);
  case sf::Keyboard::V: return action(octave, mt::Note::F);
  case sf::Keyboard::G: return action(octave, mt::Note::F_sharp);
  case sf::Keyboard::B: return action(octave, mt::Note::G);
  case sf::Keyboard::H: return action(octave, mt::Note::G_sharp);
  case sf::Keyboard::N: return action(octave, mt::Note::A);
  case sf::Keyboard::J: return action(octave, mt::Note::A_sharp);
  case sf::Keyboard::M: return action(octave, mt::Note::B);
  case sf::Keyboard::Comma: return action(octave + 1, mt::Note::C);
  case sf::Keyboard::L: return action(octave + 1, mt::Note::C_sharp);
  case sf::Keyboard::Period: return action(octave + 1, mt::Note::D);
  case sf::Keyboard::Q: return action(octave + 1, mt::Note::C);
  case sf::Keyboard::Num2: return action(octave + 1, mt::Note::C_sharp);
  case sf::Keyboard::W: return action(octave + 1, mt::Note::D);
  case sf::Keyboard::Num3: return action(octave + 1, mt::Note::D_sharp);
  case sf::Keyboard::E: return action(octave + 1, mt::Note::E);
  case sf::Keyboard::R: return action(octave + 1, mt::Note::F);
  case sf::Keyboard::Num5: return action(octave + 1, mt::Note::F_sharp);
  case sf::Keyboard::T: return action(octave + 1, mt::Note::G);
  case sf::Keyboard::Num6: return action(octave + 1, mt::Note::G_sharp);
  case sf::Keyboard::Y: return action(octave + 1, mt::Note::A);
  case sf::Keyboard::Num7: return action(octave + 1, mt::Note::A_sharp);
  case sf::Keyboard::U: return action(octave + 1, mt::Note::B);
  case sf::Keyboard::I: return action(octave + 2, mt::Note::C);
  case sf::Keyboard::Num9: return action(octave + 2, mt::Note::C_sharp);
  case sf::Keyboard::O: return action(octave + 2, mt::Note::D);
  case sf::Keyboard::Num0: return action(octave + 2, mt::Note::D_sharp);
  case sf::Keyboard::P: return action(octave + 2, mt::Note::E);
  default: return;
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
