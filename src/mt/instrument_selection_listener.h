#ifndef INSTRUMENT_SELECTION_LISTENER_H
#define INSTRUMENT_SELECTION_LISTENER_H

class Instrument;

class InstrumentSelectionListener
{
public:
  virtual ~InstrumentSelectionListener() = 0;

  virtual void selected_instrument_changed(Instrument* instr) = 0;
};

inline InstrumentSelectionListener::~InstrumentSelectionListener() = default;

#endif /* INSTRUMENT_SELECTION_LISTENER_H */
