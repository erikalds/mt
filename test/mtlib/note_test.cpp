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

TEST_CASE("note_def.to_string", "[note]")
{
  CHECK("C-4" == mt::to_string(mt::NoteDef{mt::Note::C, 4}));
  CHECK("C#5" == mt::to_string(mt::NoteDef{mt::Note::C_sharp, 5}));
  CHECK("D-6" == mt::to_string(mt::NoteDef{mt::Note::D, 6}));
  CHECK("D#7" == mt::to_string(mt::NoteDef{mt::Note::D_sharp, 7}));
  CHECK("E-0" == mt::to_string(mt::NoteDef{mt::Note::E, 0}));
  CHECK("F-1" == mt::to_string(mt::NoteDef{mt::Note::F, 1}));
  CHECK("F#2" == mt::to_string(mt::NoteDef{mt::Note::F_sharp, 2}));
  CHECK("G-3" == mt::to_string(mt::NoteDef{mt::Note::G, 3}));
  CHECK("G#4" == mt::to_string(mt::NoteDef{mt::Note::G_sharp, 4}));
  CHECK("A-5" == mt::to_string(mt::NoteDef{mt::Note::A, 5}));
  CHECK("A#6" == mt::to_string(mt::NoteDef{mt::Note::A_sharp, 6}));
  CHECK("B-7" == mt::to_string(mt::NoteDef{mt::Note::B, 7}));
}

TEST_CASE("note_def.from_string", "[note]")
{
  CHECK(mt::NoteDef::from_string("C-4") == mt::NoteDef{mt::Note::C, 4});
  CHECK(mt::NoteDef::from_string("C#5") == mt::NoteDef{mt::Note::C_sharp, 5});
  CHECK(mt::NoteDef::from_string("D-6") == mt::NoteDef{mt::Note::D, 6});
  CHECK(mt::NoteDef::from_string("D#7") == mt::NoteDef{mt::Note::D_sharp, 7});
  CHECK(mt::NoteDef::from_string("E-0") == mt::NoteDef{mt::Note::E, 0});
  CHECK(mt::NoteDef::from_string("F-1") == mt::NoteDef{mt::Note::F, 1});
  CHECK(mt::NoteDef::from_string("F#2") == mt::NoteDef{mt::Note::F_sharp, 2});
  CHECK(mt::NoteDef::from_string("G-3") == mt::NoteDef{mt::Note::G, 3});
  CHECK(mt::NoteDef::from_string("G#4") == mt::NoteDef{mt::Note::G_sharp, 4});
  CHECK(mt::NoteDef::from_string("A-5") == mt::NoteDef{mt::Note::A, 5});
  CHECK(mt::NoteDef::from_string("A#6") == mt::NoteDef{mt::Note::A_sharp, 6});
  CHECK(mt::NoteDef::from_string("B-7") == mt::NoteDef{mt::Note::B, 7});
}

TEST_CASE("note_def.fmt", "[note]")
{
  CHECK("C-5" == fmt::format("{}", mt::NoteDef{mt::Note::C, 5}));
  CHECK("C#6" == fmt::format("{}", mt::NoteDef{mt::Note::C_sharp, 6}));
  CHECK("D-7" == fmt::format("{}", mt::NoteDef{mt::Note::D, 7}));
  CHECK("D#0" == fmt::format("{}", mt::NoteDef{mt::Note::D_sharp, 0}));
  CHECK("E-1" == fmt::format("{}", mt::NoteDef{mt::Note::E, 1}));
  CHECK("F-2" == fmt::format("{}", mt::NoteDef{mt::Note::F, 2}));
  CHECK("F#3" == fmt::format("{}", mt::NoteDef{mt::Note::F_sharp, 3}));
  CHECK("G-4" == fmt::format("{}", mt::NoteDef{mt::Note::G, 4}));
  CHECK("G#5" == fmt::format("{}", mt::NoteDef{mt::Note::G_sharp, 5}));
  CHECK("A-6" == fmt::format("{}", mt::NoteDef{mt::Note::A, 6}));
  CHECK("A#7" == fmt::format("{}", mt::NoteDef{mt::Note::A_sharp, 7}));
  CHECK("B-0" == fmt::format("{}", mt::NoteDef{mt::Note::B, 0}));
}

TEST_CASE("note_def.comparability", "[note]")
{
  SECTION("less than")
  {
    CHECK(mt::NoteDef{mt::Note::C, 5} < mt::NoteDef{mt::Note::C, 6});
    CHECK(!(mt::NoteDef{mt::Note::C, 5} < mt::NoteDef{mt::Note::C, 5}));
    CHECK(!(mt::NoteDef{mt::Note::C, 6} < mt::NoteDef{mt::Note::C, 5}));
    CHECK(mt::NoteDef{mt::Note::C, 5} < mt::NoteDef{mt::Note::C_sharp, 5});
    CHECK(!(mt::NoteDef{mt::Note::C_sharp, 5} < mt::NoteDef{mt::Note::C, 5}));
  }
  SECTION("less than or equal")
  {
    CHECK(mt::NoteDef{mt::Note::C, 5} <= mt::NoteDef{mt::Note::C, 6});
    CHECK(mt::NoteDef{mt::Note::C, 5} <= mt::NoteDef{mt::Note::C, 5});
    CHECK(!(mt::NoteDef{mt::Note::C, 6} <= mt::NoteDef{mt::Note::C, 5}));
    CHECK(mt::NoteDef{mt::Note::C, 5} <= mt::NoteDef{mt::Note::C_sharp, 5});
    CHECK(!(mt::NoteDef{mt::Note::C_sharp, 5} <= mt::NoteDef{mt::Note::C, 5}));
  }
  SECTION("equal")
  {
    CHECK(!(mt::NoteDef{mt::Note::C, 5} == mt::NoteDef{mt::Note::C, 6}));
    CHECK(mt::NoteDef{mt::Note::C, 5} == mt::NoteDef{mt::Note::C, 5});
    CHECK(!(mt::NoteDef{mt::Note::C, 6} == mt::NoteDef{mt::Note::C, 5}));
    CHECK(!(mt::NoteDef{mt::Note::C, 5} == mt::NoteDef{mt::Note::C_sharp, 5}));
    CHECK(!(mt::NoteDef{mt::Note::C_sharp, 5} == mt::NoteDef{mt::Note::C, 5}));
  }
  SECTION("greater than or equal")
  {
    CHECK(!(mt::NoteDef{mt::Note::C, 5} >= mt::NoteDef{mt::Note::C, 6}));
    CHECK(mt::NoteDef{mt::Note::C, 5} >= mt::NoteDef{mt::Note::C, 5});
    CHECK(mt::NoteDef{mt::Note::C, 6} >= mt::NoteDef{mt::Note::C, 5});
    CHECK(!(mt::NoteDef{mt::Note::C, 5} >= mt::NoteDef{mt::Note::C_sharp, 5}));
    CHECK(mt::NoteDef{mt::Note::C_sharp, 5} >= mt::NoteDef{mt::Note::C, 5});
  }
  SECTION("greater than")
  {
    CHECK(!(mt::NoteDef{mt::Note::C, 5} > mt::NoteDef{mt::Note::C, 6}));
    CHECK(!(mt::NoteDef{mt::Note::C, 5} > mt::NoteDef{mt::Note::C, 5}));
    CHECK(mt::NoteDef{mt::Note::C, 6} > mt::NoteDef{mt::Note::C, 5});
    CHECK(!(mt::NoteDef{mt::Note::C, 5} > mt::NoteDef{mt::Note::C_sharp, 5}));
    CHECK(mt::NoteDef{mt::Note::C_sharp, 5} > mt::NoteDef{mt::Note::C, 5});
  }
}
