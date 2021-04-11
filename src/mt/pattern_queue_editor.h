#ifndef PATTERN_QUEUE_EDITOR_H
#define PATTERN_QUEUE_EDITOR_H

#include "digg/subwindow.h"

namespace mt { class PatternQueue; }

class PatternView;

class PatternQueueEditor : public digg::SubWindow
{
public:
  explicit PatternQueueEditor(PatternView& pv);

  void set_pattern_queue(mt::PatternQueue& pq);

  void draw_widgets() override;

private:
  void display_current_pattern();

  mt::PatternQueue* pattern_queue = nullptr;
  PatternView& pattern_view;

  int current_queue_idx = 0;
};

#endif /* PATTERN_QUEUE_EDITOR_H */
