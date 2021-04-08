#include "pattern.h"

#include <cstdint>
#include <iomanip>
#include <sstream>

namespace mt {

  namespace {

    std::string to_str(const std::optional<NoteDef>& nd, bool stop)
    {
      if (stop)
      {
        return "===";
      }
      if (!nd)
      {
        return "---";
      }
      return to_string(*nd);
    }

    template<typename T>
    std::string to_str(const std::optional<T>& v)
    {
      constexpr const auto w{sizeof(T) * 2};
      if (!v)
      {
        return std::string(w, '-');
      }

      std::ostringstream ost;
      ost << std::setw(w) << std::setfill('0') << std::hex << std::fixed
          << static_cast<int>(*v);
      return ost.str();
    }

  } // anonymous namespace

  [[nodiscard]] std::string NoteEvent::represent() const
  {
    std::string mods{};
    for (const auto& m : mod)
    {
      mods += " " + to_str(m);
    }
    return to_str(note, stop) + " " + to_str(instr) + " " + to_str(volume) + mods;
  }

}  // namespace mt
