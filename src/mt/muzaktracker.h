#ifndef MUZAKTRACKER_H
#define MUZAKTRACKER_H

#include <memory>
#include <vector>

namespace digg
{
  class Action;
  class MainWindow;
}  // namespace digg

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
  std::vector<digg::Action> file_actions;
};

#endif /* MUZAKTRACKER_H */
