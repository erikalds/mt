#ifndef MENUITEM_H
#define MENUITEM_H

namespace digg
{

  class Action;

  class MenuItem
  {
  public:
    explicit MenuItem(Action& action);
    ~MenuItem();

    void process();

  private:
    Action* action;
  };

}  // namespace digg

#endif /* MENUITEM_H */
