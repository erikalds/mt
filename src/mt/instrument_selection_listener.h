#ifndef INSTRUMENT_SELECTION_LISTENER_H
#define INSTRUMENT_SELECTION_LISTENER_H

class Instrument;
class Sample;

class InstrumentSelectionListener
{
public:
  virtual ~InstrumentSelectionListener() = 0;

  virtual void selected_instrument_changed(Instrument* instr, Sample* sample) = 0;
};

inline InstrumentSelectionListener::~InstrumentSelectionListener() = default;

#endif /* INSTRUMENT_SELECTION_LISTENER_H */
