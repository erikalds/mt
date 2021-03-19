#ifndef INSTRUMENT_EDITOR_H
#define INSTRUMENT_EDITOR_H

#include "instrument_selection_listener.h"
#include "digg/subwindow.h"

class InstrumentEditor : public digg::SubWindow,
                         public InstrumentSelectionListener
{
public:
  InstrumentEditor();

  void selected_instrument_changed(Instrument* i, mt::Sample* s) override;

  void draw_widgets() override;

private:
  Instrument* instrument = nullptr;
  mt::Sample* sample = nullptr;
};

#endif /* INSTRUMENT_EDITOR_H */
