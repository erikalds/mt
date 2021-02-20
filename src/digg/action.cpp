#include "action.h"

namespace digg
{
  Action::Action(std::string name, std::function<void()> fun_) :
    Action{std::move(name), std::move(fun_), std::optional<shortcut>{}}
  {
  }

  Action::Action(std::string name, std::function<void()> fun_,
                 std::optional<shortcut> sc) :
    name_{std::move(name)},
    fun{std::move(fun_)},
    shortcut_{std::move(sc)}
  {
  }

  void Action::set_shortcut(sf::Keyboard::Key code, bool control, bool alt)
  {
    shortcut_ = shortcut(code, control, alt);
  }

  void Action::process_event(const sf::Event& event) const
  {
    if (!shortcut_)
    {
      return;
    }

    if (shortcut_->triggered(event))
    {
      act();
    }
  }

  std::optional<std::string_view> Action::shortcut_text() const
  {
    if (!shortcut_)
    {
      return std::optional<std::string_view>{};
    }

    return shortcut_->text();
  }

  void Action::act() const
  {
    fun();
  }

  namespace
  {
    std::string generate_shortcut_text(sf::Keyboard::Key code, bool ctrl, bool alt)
    {
      std::string s;
      if (ctrl)
      {
        s = "Ctrl";
      }
      if (alt)
      {
        if (ctrl)
        {
          s += "+";
        }
        s += "Alt";
      }
      if (!s.empty())
      {
        s += "+";
      }

      switch (code)
      {
      case sf::Keyboard::A: s += "A"; break;
      case sf::Keyboard::B: s += "B"; break;
      case sf::Keyboard::C: s += "C"; break;
      case sf::Keyboard::D: s += "D"; break;
      case sf::Keyboard::E: s += "E"; break;
      case sf::Keyboard::F: s += "F"; break;
      case sf::Keyboard::G: s += "G"; break;
      case sf::Keyboard::H: s += "H"; break;
      case sf::Keyboard::I: s += "I"; break;
      case sf::Keyboard::J: s += "J"; break;
      case sf::Keyboard::K: s += "K"; break;
      case sf::Keyboard::L: s += "L"; break;
      case sf::Keyboard::M: s += "M"; break;
      case sf::Keyboard::N: s += "N"; break;
      case sf::Keyboard::O: s += "O"; break;
      case sf::Keyboard::P: s += "P"; break;
      case sf::Keyboard::Q: s += "Q"; break;
      case sf::Keyboard::R: s += "R"; break;
      case sf::Keyboard::S: s += "S"; break;
      case sf::Keyboard::T: s += "T"; break;
      case sf::Keyboard::U: s += "U"; break;
      case sf::Keyboard::V: s += "V"; break;
      case sf::Keyboard::W: s += "W"; break;
      case sf::Keyboard::X: s += "X"; break;
      case sf::Keyboard::Y: s += "Y"; break;
      case sf::Keyboard::Z: s += "Z"; break;
      case sf::Keyboard::Num0: s += "0"; break;
      case sf::Keyboard::Num1: s += "1"; break;
      case sf::Keyboard::Num2: s += "2"; break;
      case sf::Keyboard::Num3: s += "3"; break;
      case sf::Keyboard::Num4: s += "4"; break;
      case sf::Keyboard::Num5: s += "5"; break;
      case sf::Keyboard::Num6: s += "6"; break;
      case sf::Keyboard::Num7: s += "7"; break;
      case sf::Keyboard::Num8: s += "8"; break;
      case sf::Keyboard::Num9: s += "9"; break;
      case sf::Keyboard::Escape: s += "Escape"; break;
      case sf::Keyboard::LControl: s += "LControl"; break;
      case sf::Keyboard::LShift: s += "LShift"; break;
      case sf::Keyboard::LAlt: s += "LAlt"; break;
      case sf::Keyboard::LSystem: s += "LSystem"; break;
      case sf::Keyboard::RControl: s += "RControl"; break;
      case sf::Keyboard::RShift: s += "RShift"; break;
      case sf::Keyboard::RAlt: s += "RAlt"; break;
      case sf::Keyboard::RSystem: s += "RSystem"; break;
      case sf::Keyboard::Menu: s += "Menu"; break;
      case sf::Keyboard::LBracket: s += "LBracket"; break;
      case sf::Keyboard::RBracket: s += "RBracket"; break;
      case sf::Keyboard::Semicolon: s += "Semicolon"; break;
      case sf::Keyboard::Comma: s += "Comma"; break;
      case sf::Keyboard::Period: s += "Period"; break;
      case sf::Keyboard::Quote: s += "Quote"; break;
      case sf::Keyboard::Slash: s += "Slash"; break;
      case sf::Keyboard::Backslash: s += "Backslash"; break;
      case sf::Keyboard::Tilde: s += "Tilde"; break;
      case sf::Keyboard::Equal: s += "Equal"; break;
      case sf::Keyboard::Hyphen: s += "Hyphen"; break;
      case sf::Keyboard::Space: s += "Space"; break;
      case sf::Keyboard::Enter: s += "Enter"; break;
      case sf::Keyboard::Backspace: s += "Backspace"; break;
      case sf::Keyboard::Tab: s += "Tab"; break;
      case sf::Keyboard::PageUp: s += "PageUp"; break;
      case sf::Keyboard::PageDown: s += "PageDown"; break;
      case sf::Keyboard::End: s += "End"; break;
      case sf::Keyboard::Home: s += "Home"; break;
      case sf::Keyboard::Insert: s += "Insert"; break;
      case sf::Keyboard::Delete: s += "Delete"; break;
      case sf::Keyboard::Add: s += "Add"; break;
      case sf::Keyboard::Subtract: s += "Subtract"; break;
      case sf::Keyboard::Multiply: s += "Multiply"; break;
      case sf::Keyboard::Divide: s += "Divide"; break;
      case sf::Keyboard::Left: s += "Left"; break;
      case sf::Keyboard::Right: s += "Right"; break;
      case sf::Keyboard::Up: s += "Up"; break;
      case sf::Keyboard::Down: s += "Down"; break;
      case sf::Keyboard::Numpad0: s += "0"; break;
      case sf::Keyboard::Numpad1: s += "1"; break;
      case sf::Keyboard::Numpad2: s += "2"; break;
      case sf::Keyboard::Numpad3: s += "3"; break;
      case sf::Keyboard::Numpad4: s += "4"; break;
      case sf::Keyboard::Numpad5: s += "5"; break;
      case sf::Keyboard::Numpad6: s += "6"; break;
      case sf::Keyboard::Numpad7: s += "7"; break;
      case sf::Keyboard::Numpad8: s += "8"; break;
      case sf::Keyboard::Numpad9: s += "9"; break;
      case sf::Keyboard::F1: s += "F1"; break;
      case sf::Keyboard::F2: s += "F2"; break;
      case sf::Keyboard::F3: s += "F3"; break;
      case sf::Keyboard::F4: s += "F4"; break;
      case sf::Keyboard::F5: s += "F5"; break;
      case sf::Keyboard::F6: s += "F6"; break;
      case sf::Keyboard::F7: s += "F7"; break;
      case sf::Keyboard::F8: s += "F8"; break;
      case sf::Keyboard::F9: s += "F9"; break;
      case sf::Keyboard::F10: s += "F10"; break;
      case sf::Keyboard::F11: s += "F11"; break;
      case sf::Keyboard::F12: s += "F12"; break;
      case sf::Keyboard::F13: s += "F13"; break;
      case sf::Keyboard::F14: s += "F14"; break;
      case sf::Keyboard::F15: s += "F15"; break;
      case sf::Keyboard::Pause: s += "Pause"; break;

      case sf::Keyboard::Unknown: s += "Unknown"; break;
      case sf::Keyboard::KeyCount: s += "KeyCount"; break;
      }
      return s;
    }
  }  // anonymous namespace

  Action::shortcut::shortcut(sf::Keyboard::Key code_, bool control_, bool alt_) :
    code(code_),
    control(control_),
    alt(alt_),
    text_(generate_shortcut_text(code_, control_, alt_))
  {
  }

  bool Action::shortcut::triggered(const sf::Event& e) const
  {
    return e.type == sf::Event::KeyPressed
      && e.key.code == this->code
      && e.key.control == this->control
      && e.key.alt == this->alt;
  }

}  // namespace digg
