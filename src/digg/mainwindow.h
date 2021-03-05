#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainmenubar.h"
#include <SFML/Graphics/View.hpp>
#include <string>
#include <memory>
#include <vector>

namespace sf { class RenderWindow; class Time; }

namespace digg
{
  class EventProcessor;
  class Renderable;
  class Widget;

  class MainWindow
  {
  public:
    explicit MainWindow(const std::string& title);
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow(MainWindow&&) noexcept;
    MainWindow& operator=(MainWindow&&) noexcept;
    ~MainWindow();

    void add_widget(Widget& w);
    void remove_widget(Widget& w);
    void add_renderable(Renderable& r);

    void add_menu(const Menu& m);

    void exec();

    EventProcessor& get_eventprocessor() { return *ep; }
    void close();

    sf::View& world_view() { return world_view_; }

  private:
    void process_events();
    void update_objects(const sf::Time& delta_time);
    void render_window();

    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<EventProcessor> ep;
    MainMenuBar menubar;
    std::vector<Widget*> widgets;
    std::vector<Renderable*> renderables;
    sf::View world_view_;
  };

}  // namespace digg

#endif /* MAINWINDOW_H */
