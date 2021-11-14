#include "muzaktracker.h"

#include "instrument_editor.h"
#include "instrument_list.h"
#include "keyboard.h"
#include "pattern_queue_editor.h"
#include "pattern_view.h"
#include "mtlib/config.h"
#include "mtlib/project.h"

#include "sndmix/mixer.h"
#include "digg/action.h"
#include "digg/event_processor.h"
#include "digg/file_dialog.h"
#include "digg/mainwindow.h"
#include "digg/menu.h"
#include <SFML/Window/Keyboard.hpp>
#include <spdlog/spdlog.h>

MuzakTracker::MuzakTracker(mt::Config& cfg, std::unique_ptr<mt::snd::Mixer> mixer_) :
  main_window{std::make_unique<digg::MainWindow>("Muzak Tracker")},
  keyboard{std::make_unique<Keyboard>()},
  instrument_list{std::make_unique<InstrumentList>()},
  instrument_editor{std::make_unique<InstrumentEditor>()},
  pattern_view{std::make_unique<PatternView>()},
  pattern_queue_editor{std::make_unique<PatternQueueEditor>(*pattern_view)},
  mixer{std::move(mixer_)},
  config{cfg}
{
  main_window->set_size(config.get_window_size());

  keyboard->connect_to(*mixer);

  set_current_project(std::make_unique<mt::Project>());
  instrument_list->add_selection_listener(*keyboard);
  instrument_list->add_selection_listener(*instrument_editor);
  instrument_list->add_selection_listener(*pattern_view);

  pattern_view->set_keyboard(*keyboard);

  main_window->add_widget(*keyboard);
  main_window->add_widget(*instrument_list);
  main_window->add_widget(*instrument_editor);
  main_window->add_widget(*pattern_view);
  main_window->add_widget(*pattern_queue_editor);
  main_window->get_eventprocessor().add_eventlistener(*keyboard);
  main_window->get_eventprocessor().add_eventlistener(*pattern_view);

  create_actions();
  create_menubar();
}

MuzakTracker::~MuzakTracker()
{
  config.set_window_size(main_window->get_size());
}

int MuzakTracker::run()
{
  main_window->exec();
  return 0;
}

void MuzakTracker::load_project(const std::filesystem::path& p)
{
  open_project(p);
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
  file_actions.emplace_back(Action{"Save",
                                   [this]()
                                   {
                                     spdlog::info("Save");
                                     this->project->save();
                                   },
                                   Action::shortcut(sf::Keyboard::S, true, false)});
  file_actions.emplace_back(Action{"Save as...",
                                   [this]()
                                   {
                                     spdlog::info("Save as...");
                                     auto close_dialog
                                       = [this]()
                                         {
                                           this->main_window->remove_widget(*this->file_dialog);
                                         };
                                     auto save
                                       = [this, close_dialog](const std::filesystem::path& filename)
                                         {
                                           close_dialog();
                                           this->save_project_as(filename);
                                         };
                                     this->file_dialog
                                       = std::make_unique<digg::FileDialog>("Save project as...",
                                                                            "*.mzt",
                                                                            save,
                                                                            close_dialog);
                                     this->main_window->add_widget(*this->file_dialog);
                                   }});
  file_actions.emplace_back(Action{"Quit",
                                   [this]()
                                   {
                                     spdlog::info("Quit");
                                     main_window->close();
                                   },
                                   Action::shortcut(sf::Keyboard::Q, true, false)});
  view_actions.emplace_back(Action{"Toggle pattern view",
                                   [this]()
                                   {
                                     spdlog::info("Toggle pattern view");
                                     pattern_view->toggle_active();
                                   },
                                   Action::shortcut(sf::Keyboard::F8, false, false)});
  view_actions.emplace_back(Action{"Toggle pattern queue editor",
                                   [this]()
                                   {
                                     spdlog::info("Toggle pattern queue editor");
                                     pattern_queue_editor->toggle_active();
                                   },
                                   Action::shortcut(sf::Keyboard::F9, false, false)});
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
  view_actions.emplace_back(Action{"Toggle instrument editor",
                                   [this]()
                                   {
                                     spdlog::info("Toggle instrument editor");
                                     instrument_editor->toggle_active();
                                   },
                                   Action::shortcut(sf::Keyboard::F12, false, false)});
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
  auto new_proj = std::make_unique<mt::Project>();
  instrument_list->set_current_project(*new_proj);
  new_proj->load_from_file(filename);
  set_current_project(std::move(new_proj));
}

void MuzakTracker::save_project_as(const std::filesystem::path& filename)
{
  spdlog::info("Save project: {}", filename.string());
  project->save_as(filename);
}

void MuzakTracker::set_current_project(std::unique_ptr<mt::Project> p)
{
  project = std::move(p);

  instrument_list->set_current_project(*project);
  pattern_queue_editor->set_pattern_queue(project->get_pattern_queue());
  main_window->set_subtitle(std::string{project->get_title()}
                            + " [" + project->get_filename().string() + "]");
}
