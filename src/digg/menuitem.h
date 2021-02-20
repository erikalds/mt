#ifndef MENUITEM_H
#define MENUITEM_H

namespace digg
{

  class Action;

  class MenuItem
  {
  public:
    explicit MenuItem(Action& action);

    void process();

  private:
    Action* action = nullptr;
  };

}  // namespace digg

#endif /* MENUITEM_H */
