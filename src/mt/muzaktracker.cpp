#include "muzaktracker.h"

#include "instrument_list.h"
#include "keyboard.h"
#include "project.h"

#include "digg/action.h"
#include "digg/event_processor.h"
#include "digg/mainwindow.h"
#include "digg/menu.h"
#include <spdlog/spdlog.h>

MuzakTracker::MuzakTracker() :
  main_window{std::make_unique<digg::MainWindow>("Muzak Tracker")},
  keyboard{std::make_unique<Keyboard>()},
  instrument_list{std::make_unique<InstrumentList>()},
  project{std::make_unique<Project>("untitled.mzt")}
{
  instrument_list->set_current_project(*project);
  instrument_list->add_selection_listener(*keyboard);

  main_window->add_widget(*keyboard);
  main_window->add_widget(*instrument_list);
  main_window->get_eventprocessor().add_eventlistener(*keyboard);

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
  view_actions.emplace_back(Action{"Toggle keyboard",
                                   [this]()
                                   {
                                     spdlog::info("Toggle keyboard");
                                     keyboard->toggle_active();
                                   },
                                   Action::shortcut(sf::Keyboard::F10, false, false)});
  view_actions.emplace_back(Action{"Toggle instrument list",
                                   [this]()
                                   {
                                     spdlog::info("Toggle instrument list");
                                     instrument_list->toggle_active();
                                   },
                                   Action::shortcut(sf::Keyboard::F11, false, false)});
}

void MuzakTracker::create_menubar()
{
  auto add_menu = [this](std::string_view name,
                         std::vector<digg::Action>& actions)
                  {
                    digg::Menu menu{name.data()};
                    auto& ep = this->main_window->get_eventprocessor();
                    for (auto& a : actions)
                    {
                      menu.add_menuitem(digg::MenuItem{a});
                      ep.add_action(a);
                    }

                    this->main_window->add_menu(menu);
                  };

  add_menu("File", file_actions);
  add_menu("View", view_actions);
}
