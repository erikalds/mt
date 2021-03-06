#ifndef ACTION_H
#define ACTION_H

#include <SFML/Window/Event.hpp>
#include <functional>
#include <optional>

namespace digg
{

class Action
{
public:
  class shortcut;

  Action(std::string name, std::function<void()> fun_);
  Action(std::string name, std::function<void()> fun_,
         std::optional<shortcut> sc);

  void set_shortcut(sf::Keyboard::Key code, bool control, bool alt);
  void process_event(const sf::Event& event) const;

  [[nodiscard]] std::string_view name() const { return name_; }
  [[nodiscard]] std::optional<std::string_view> shortcut_text() const;

  void act() const;

  class shortcut
  {
  public:
    shortcut(sf::Keyboard::Key code, bool control, bool alt);

    [[nodiscard]] bool triggered(const sf::Event& e) const;
    [[nodiscard]] std::string_view text() const { return text_; }

  private:
    sf::Keyboard::Key code;
    bool control;
    bool alt;
    std::string text_;
  };

private:
  std::string name_;
  std::function<void()> fun;
  std::optional<shortcut> shortcut_;
};

}  // namespace digg

#endif /* ACTION_H */
