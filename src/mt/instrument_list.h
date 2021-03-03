#ifndef INSTRUMENT_LIST_H
#define INSTRUMENT_LIST_H

#include "digg/subwindow.h"
#include <set>

class InstrumentSelectionListener;
class Project;

class InstrumentList : public digg::SubWindow
{
public:
  InstrumentList();

  void set_current_project(Project& proj);
  void draw_widgets() override;

  void add_selection_listener(InstrumentSelectionListener& listener);
  void remove_selection_listener(InstrumentSelectionListener& listener);

private:
  void notify_listeners() const;

  Project* project = nullptr;
  int current_item = 0;

  std::set<InstrumentSelectionListener*> listeners;
};

#endif /* INSTRUMENT_LIST_H */
