#include "mtlib/note.h"

#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("note.to_string", "[note]")
{
  CHECK("C" == mt::to_string(mt::Note::C));
  CHECK("C#" == mt::to_string(mt::Note::C_sharp));
  CHECK("D" == mt::to_string(mt::Note::D));
  CHECK("D#" == mt::to_string(mt::Note::D_sharp));
  CHECK("E" == mt::to_string(mt::Note::E));
  CHECK("F" == mt::to_string(mt::Note::F));
  CHECK("F#" == mt::to_string(mt::Note::F_sharp));
  CHECK("G" == mt::to_string(mt::Note::G));
  CHECK("G#" == mt::to_string(mt::Note::G_sharp));
  CHECK("A" == mt::to_string(mt::Note::A));
  CHECK("A#" == mt::to_string(mt::Note::A_sharp));
  CHECK("B" == mt::to_string(mt::Note::B));
}

TEST_CASE("note.ostream_operator", "[note]")
{
  std::ostringstream ost{};
  constexpr const int note_count{12};
  for (int i = 0; i < note_count; ++i)
  {
    ost << mt::Note{i} << " ";
  }

  CHECK("C C# D D# E F F# G G# A A# B " == ost.str());
}
