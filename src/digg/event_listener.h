#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

namespace sf { class Event; }

namespace digg
{

  class EventListener
  {
  public:
    virtual ~EventListener() = 0;

    virtual void event_occurred(const sf::Event& e) = 0;
  };

}  // namespace digg

inline digg::EventListener::~EventListener() {}

#endif /* EVENT_LISTENER_H */
