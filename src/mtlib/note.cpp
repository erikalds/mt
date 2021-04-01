#include "note.h"

#include <cassert>
#include <ostream>
#include <regex>

namespace mt
{

  std::ostream& operator<<(std::ostream& out, const Note& n)
  {
    return out << to_string(n);
  }

  std::string_view to_string(const Note& n)
  {
    switch(n)
    {
    case Note::C: return "C";
    case Note::C_sharp: return "C#";
    case Note::D: return "D";
    case Note::D_sharp: return "D#";
    case Note::E: return "E";
    case Note::F: return "F";
    case Note::F_sharp: return "F#";
    case Note::G: return "G";
    case Note::G_sharp: return "G#";
    case Note::A: return "A";
    case Note::A_sharp: return "A#";
    case Note::B: return "B";
    }
    return "<unknown Note>";
  }

  namespace
  {

    using svmatch = std::match_results<std::string_view::const_iterator>;
    using svsub_match = std::sub_match<std::string_view::const_iterator>;

    inline std::string_view get_sv(const svsub_match& m)
    {
      return std::string_view(m.first, static_cast<std::size_t>(m.length()));
    }

    inline bool
    regex_match(std::string_view sv,
                svmatch& m,
                const std::regex& e,
                std::regex_constants::match_flag_type flags=std::regex_constants::match_default)
    {
      return std::regex_match(sv.begin(), sv.end(), m, e, flags);
    }

    // inline bool
    // regex_match(std::string_view sv,
    //             const std::regex& e,
    //             std::regex_constants::match_flag_type flags=std::regex_constants::match_default)
    // {
    //   return std::regex_match(sv.begin(), sv.end(), e, flags);
    // }

  }  // anonymous namespace

  NoteDef NoteDef::from_string(std::string_view s)
  {
    const std::regex rx{"([A-G][#\\-])([0-7])"};
    svmatch mo;

    if (!regex_match(s, mo, rx))
    {
      throw std::logic_error{"Unable to match NoteDef string def.: "
                               + std::string{s}};
    }

    auto notestr = get_sv(mo[1]);
    int n = (static_cast<int>(notestr[0]) - static_cast<int>('C')) * 2;
    constexpr const int notecount{12};
    if (n < 0)
    {
      n += notecount + 1;
    }
    else if (n >= 6) // no semi-note between E and F
    {
      n -= 1;
    }
    if (notestr[1] == '#')
    {
      ++n;
    }
    auto octave = static_cast<std::size_t>(get_sv(mo[2])[0] - '0');
    return NoteDef{Note{n}, octave};
  }

  std::string to_string(const NoteDef& nd)
  {
    switch(nd.note)
    {
    case Note::C: return fmt::format("C-{}", nd.octave);
    case Note::C_sharp: return fmt::format("C#{}", nd.octave);
    case Note::D: return fmt::format("D-{}", nd.octave);
    case Note::D_sharp: return fmt::format("D#{}", nd.octave);
    case Note::E: return fmt::format("E-{}", nd.octave);
    case Note::F: return fmt::format("F-{}", nd.octave);
    case Note::F_sharp: return fmt::format("F#{}", nd.octave);
    case Note::G: return fmt::format("G-{}", nd.octave);
    case Note::G_sharp: return fmt::format("G#{}", nd.octave);
    case Note::A: return fmt::format("A-{}", nd.octave);
    case Note::A_sharp: return fmt::format("A#{}", nd.octave);
    case Note::B: return fmt::format("B-{}", nd.octave);
    }
    return fmt::format("X-{}", nd.octave);
  }

  std::ostream& operator<<(std::ostream& out, const NoteDef& n)
  {
    return out << to_string(n);
  }

}  // namespace mt
