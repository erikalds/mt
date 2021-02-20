#ifndef WIDGET_H
#define WIDGET_H

namespace digg
{

  class Widget
  {
  public:
    virtual ~Widget() = 0;

    virtual void process() = 0;
  };

}  // namespace digg

inline digg::Widget::~Widget() = default;

#endif /* WIDGET_H */
