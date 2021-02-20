#include "mainwindow.h"

#include "event_processor.h"
#include "renderable.h"
#include "widget.h"
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <spdlog/spdlog.h>

namespace digg
{

  MainWindow::MainWindow(std::string title) :
    window{std::make_unique<sf::RenderWindow>(sf::VideoMode{640, 480}, title.c_str())},
    ep{std::make_unique<EventProcessor>()}
  {
    window->setFramerateLimit(60);
    ImGui::SFML::Init(*window);

    widgets.push_back(&menubar);
  }

  MainWindow::MainWindow(MainWindow&&) = default;

  MainWindow& MainWindow::operator=(MainWindow&&) = default;

  MainWindow::~MainWindow()
  {
    ImGui::SFML::Shutdown();
  }

  void MainWindow::add_widget(Widget& w)
  {
    widgets.push_back(&w);
  }

  void MainWindow::add_renderable(Renderable& r)
  {
    renderables.push_back(&r);
  }

  void MainWindow::add_menu(const Menu& m)
  {
    menubar.add_menu(m);
  }

  void MainWindow::exec()
  {
    sf::Clock deltaClock;
    while (window->isOpen())
    {
      process_events();
      update_objects(deltaClock.restart());
      render_window();
    }
  }

  void MainWindow::close()
  {
    window->close();
  }

  void MainWindow::process_events()
  {
    sf::Event event;
    while (window->pollEvent(event))
    {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed)
      {
        close();
      }
      else if (event.type == sf::Event::Resized)
      {
        auto w = static_cast<float>(event.size.width);
        auto h = static_cast<float>(event.size.height);
        world_view_.setSize(w, h);
        world_view_.setCenter(w / 2, h / 2);
        spdlog::debug("Resize world view: {} x {}", w, h);
      }

      ep->process_event(event);
    }
  }

  void MainWindow::update_objects(const sf::Time& delta_time)
  {
    ImGui::SFML::Update(*window, delta_time);

    for (auto* r : renderables)
      r->update(delta_time);

    for (auto* w : widgets)
      w->process();
  }

  void MainWindow::render_window()
  {
    window->clear();

    window->setView(world_view_);
    for (auto* r : renderables)
      r->draw_to(*window);

    window->setView(window->getDefaultView());
    ImGui::SFML::Render(*window);

    window->display();
  }

}  // namespace digg