#ifndef MUZAKTRACKER_H
#define MUZAKTRACKER_H

#include <filesystem>
#include <memory>
#include <vector>

namespace digg
{
  class Action;
  class FileDialog;
  class MainWindow;
}  // namespace digg

namespace mt {
  class Config;
  class Project;
}

class InstrumentEditor;
class InstrumentList;
class Keyboard;
class PatternQueueEditor;
class PatternView;

class MuzakTracker
{
public:
  explicit MuzakTracker(mt::Config& cfg);
  MuzakTracker(const MuzakTracker&) = delete;
  MuzakTracker& operator=(const MuzakTracker&) = delete;
  MuzakTracker(MuzakTracker&&) = delete;
  MuzakTracker& operator=(MuzakTracker&&) = delete;
  ~MuzakTracker();

  int run();
  void load_project(const std::filesystem::path& p);

private:
  void create_actions();
  void create_menubar();

  void open_project(const std::filesystem::path& filename);
  void save_project_as(const std::filesystem::path& filename);
  void set_current_project(std::unique_ptr<mt::Project> p);

  std::unique_ptr<digg::MainWindow> main_window;
  std::unique_ptr<Keyboard> keyboard;
  std::unique_ptr<InstrumentList> instrument_list;
  std::unique_ptr<InstrumentEditor> instrument_editor;
  std::unique_ptr<PatternView> pattern_view;
  std::unique_ptr<PatternQueueEditor> pattern_queue_editor;
  std::unique_ptr<mt::Project> project;
  std::vector<digg::Action> file_actions;
  std::vector<digg::Action> view_actions;
  std::unique_ptr<digg::FileDialog> file_dialog;
  mt::Config& config;
};

#endif /* MUZAKTRACKER_H */
