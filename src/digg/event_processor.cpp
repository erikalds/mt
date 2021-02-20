#include "event_processor.h"

#include "action.h"
#include "event_listener.h"

namespace digg
{
  void EventProcessor::add_action(Action& a)
  {
    actions.insert(&a);
  }

  void EventProcessor::add_eventlistener(EventListener& listener)
  {
    listeners.insert(&listener);
  }

  void EventProcessor::process_event(const sf::Event& e) const
  {
    for (const auto* a : actions)
    {
      a->process_event(e);
    }

    for (auto* listener : listeners)
    {
      listener->event_occurred(e);
    }
  }
}  // namespace digg
