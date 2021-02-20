#ifndef EVENT_PROCESSOR_H
#define EVENT_PROCESSOR_H

#include <set>

namespace sf { class Event; }

namespace digg
{
  class Action;
  class EventListener;

  class EventProcessor
  {
  public:
    void add_action(Action& a);
    void add_eventlistener(EventListener& listener);

    void process_event(const sf::Event& e) const;

  private:
    std::set<Action*> actions;
    std::set<EventListener*> listeners;
  };
}  // namespace digg

#endif /* EVENT_PROCESSOR_H */
