#ifndef MUZAKTRACKER_H
#define MUZAKTRACKER_H

#include <memory>
#include <vector>

namespace digg
{
  class Action;
  class MainWindow;
}  // namespace digg

class InstrumentList;
class Keyboard;
class Project;

class MuzakTracker
{
public:
  MuzakTracker();
  MuzakTracker(const MuzakTracker&) = delete;
  MuzakTracker& operator=(const MuzakTracker&) = delete;
  MuzakTracker(MuzakTracker&&) = delete;
  MuzakTracker& operator=(MuzakTracker&&) = delete;
  ~MuzakTracker();

  int run();

private:
  void create_actions();
  void create_menubar();

  std::unique_ptr<digg::MainWindow> main_window;
  std::unique_ptr<Keyboard> keyboard;
  std::unique_ptr<InstrumentList> instrument_list;
  std::unique_ptr<Project> project;
  std::vector<digg::Action> file_actions;
  std::vector<digg::Action> view_actions;
};

#endif /* MUZAKTRACKER_H */
