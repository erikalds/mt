#ifndef INSTRUMENT_LIST_H
#define INSTRUMENT_LIST_H

#include "digg/subwindow.h"
#include <set>

namespace mt { class Project; }

class InstrumentSelectionListener;

class InstrumentList : public digg::SubWindow
{
public:
  InstrumentList();

  void set_current_project(mt::Project& proj);
  void draw_widgets() override;

  void add_selection_listener(InstrumentSelectionListener& listener);
  void remove_selection_listener(InstrumentSelectionListener& listener);

private:
  void notify_listeners() const;
  void notify_listener(InstrumentSelectionListener& listener) const;

  mt::Project* project = nullptr;
  int current_instrument = 0;
  int current_sample = 0;

  std::set<InstrumentSelectionListener*> listeners;
};

#endif /* INSTRUMENT_LIST_H */
