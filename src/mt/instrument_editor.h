#ifndef INSTRUMENT_EDITOR_H
#define INSTRUMENT_EDITOR_H

#include "instrument_selection_listener.h"
#include "sample_view.h"
#include "digg/subwindow.h"

class InstrumentEditor : public digg::SubWindow,
                         public InstrumentSelectionListener
{
public:
  InstrumentEditor();

  void selected_instrument_changed(int instr_idx, mt::Instrument* i,
                                   mt::Sample* s) override;

  void draw_widgets() override;

private:
  mt::Instrument* instrument = nullptr;
  mt::Sample* sample = nullptr;
  SampleView sample_view;
};

#endif /* INSTRUMENT_EDITOR_H */
