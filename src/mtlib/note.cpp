#include "note.h"

#include <ostream>

namespace mt {

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

}  // namespace mt
