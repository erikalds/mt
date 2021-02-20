#ifndef RENDERABLE_H
#define RENDERABLE_H

namespace sf
{
  class RenderWindow;
  class Time;
}  // namespace sf

namespace digg
{

  class Renderable
  {
  public:
    virtual ~Renderable() = 0;

    virtual void update(const sf::Time& delta_time) = 0;
    virtual void draw_to(sf::RenderWindow& window) = 0;
  };

}  // namespace digg

inline digg::Renderable::~Renderable() {}

#endif /* RENDERABLE_H */
