#include "muzaktracker.h"

#include "instrument_list.h"
#include "keyboard.h"
#include "project.h"

#include "digg/action.h"
#include "digg/event_processor.h"
#include "digg/file_dialog.h"
#include "digg/mainwindow.h"
#include "digg/menu.h"
#include <SFML/Window/Keyboard.hpp>
#include <spdlog/spdlog.h>

MuzakTracker::MuzakTracker() :
  main_window{std::make_unique<digg::MainWindow>("Muzak Tracker")},
  keyboard{std::make_unique<Keyboard>()},
  instrument_list{std::make_unique<InstrumentList>()}
{
  set_current_project(std::make_unique<Project>());
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
  file_actions.emplace_back(Action{"Open",
                                   [this]()
                                   {
                                     spdlog::info("Open");
                                     auto close_dialog
                                       = [this]()
                                         {
                                           this->main_window->remove_widget(*this->file_dialog);
                                         };
                                     auto open
                                       = [this, close_dialog](const std::filesystem::path& filename)
                                         {
                                           close_dialog();
                                           this->open_project(filename);
                                         };
                                     this->file_dialog
                                       = std::make_unique<digg::FileDialog>("Open project",
                                                                            "*.mzt",
                                                                            open,
                                                                            close_dialog);
                                     this->main_window->add_widget(*this->file_dialog);
                                   },
                                   Action::shortcut(sf::Keyboard::O, true, false)});
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

void MuzakTracker::open_project(const std::filesystem::path& filename)
{
  spdlog::info("Open project: {}", filename.string());
  auto new_proj = std::make_unique<Project>();
  instrument_list->set_current_project(*new_proj);
  new_proj->load_from_file(filename);
  set_current_project(std::move(new_proj));
}

void MuzakTracker::set_current_project(std::unique_ptr<Project> p)
{
  project = std::move(p);

  instrument_list->set_current_project(*project);
  main_window->set_subtitle(project->get_title());
}
