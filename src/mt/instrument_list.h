#ifndef INSTRUMENT_LIST_H
#define INSTRUMENT_LIST_H

#include "digg/subwindow.h"
#include <memory>
#include <set>

namespace digg { class FileDialog; }
namespace mt { class Instrument; class Project; }

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
  [[nodiscard]] const mt::Instrument* get_current_instrument() const;
  [[nodiscard]] mt::Instrument* get_current_instrument();
  void add_instrument();
  void remove_current_instrument();
  void add_sample();
  void remove_current_sample();

  void notify_listeners();
  void notify_listener(InstrumentSelectionListener& listener);

  mt::Project* project = nullptr;
  int current_instrument = 0;
  int current_sample = 0;

  std::set<InstrumentSelectionListener*> listeners;
  std::unique_ptr<digg::FileDialog> file_dialog;
};

#endif /* INSTRUMENT_LIST_H */
