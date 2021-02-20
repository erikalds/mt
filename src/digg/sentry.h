#ifndef SENTRY_H
#define SENTRY_H

#include <functional>

namespace digg
{

  /// Simple sentry class that calls the given function object in destructor.
  class Sentry
  {
  public:
    explicit Sentry(std::function<void()> call_in_dtor) : fun(std::move(call_in_dtor)) {}
    Sentry(const Sentry&) = delete;
    Sentry& operator=(const Sentry&) = delete;
    Sentry(Sentry&&) = default;
    Sentry& operator=(Sentry&&) = default;
    ~Sentry() { fun(); }

  private:
    std::function<void()> fun;
  };

}  // namespace digg

#endif /* SENTRY_H */
