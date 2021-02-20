#include "muzaktracker.h"

#include "digg/action.h"
#include "digg/event_processor.h"
#include "digg/mainwindow.h"
#include "digg/menu.h"
#include <spdlog/spdlog.h>

MuzakTracker::MuzakTracker() :
  main_window{std::make_unique<digg::MainWindow>("Muzak Tracker")}
{
  create_actions();
  create_menubar();
}

MuzakTracker::~MuzakTracker() = default;

int MuzakTracker::run()
{
  main_window->exec();
  return 0;
}

void MuzakTracker::create_actions()
{
  using digg::Action;
  file_actions.emplace_back(Action{"Quit",
                                   [this]()
                                   {
                                     spdlog::info("Quit");
                                     main_window->close();
                                   },
                                   Action::shortcut(sf::Keyboard::Q, true, false)});

  auto& ep = main_window->get_eventprocessor();
  for (auto& a : file_actions)
  {
    ep.add_action(a);
  }
}

void MuzakTracker::create_menubar()
{
  auto add_menu = [this](std::string_view name,
                         std::vector<digg::Action>& actions)
                  {
                    digg::Menu menu{name.data()};
                    for (auto& a : actions)
                    {
                      menu.add_menuitem(digg::MenuItem{a});
                    }

                    this->main_window->add_menu(menu);
                  };

  add_menu("File", file_actions);
}
