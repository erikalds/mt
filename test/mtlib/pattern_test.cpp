#include "mtlib/pattern.h"

#include <catch2/catch.hpp>

TEST_CASE("Pattern_Track_containers", "[pattern]")
{
  mt::Pattern p0{0x40, 8};
  mt::Pattern p1{0x60, 12};
  CHECK(8 == p0.size());
  CHECK(8 == std::distance(p0.begin(), p0.end()));
  CHECK(12 == std::distance(p1.begin(), p1.end()));
  CHECK(0x40 == p0.get_track_length());
  CHECK(0x60 == p1.get_track_length());
  for (auto i = 0u; i < p0.size(); ++i)
  {
    CHECK(0x40 == p0[i].size());
    CHECK(0x40 == std::distance(p0[i].begin(), p0[i].end()));
    for (auto j = 0u; j < p0[i].size(); ++j)
    {
      CHECK("--- -- -- ----" == p0[i][j].represent());
    }
  }
  CHECK(12 == p1.size());
  for (const auto& track : p1)
  {
    CHECK(0x60 == track.size());
    CHECK(0x60 == std::distance(track.begin(), track.end()));
    for (const auto& note_event : track)
    {
      CHECK("--- -- -- ----" == note_event.represent());
    }
  }
}

TEST_CASE("NoteEvent_represent", "[pattern]")
{
  mt::NoteEvent n0{1};
  n0.note = mt::NoteDef{mt::Note::C, 5};
  n0.instr = 1;
  n0.volume = 0x80;
  CHECK("C-5 01 80 ----" == n0.represent());
  n0.mod[0] = 0x9000;
  CHECK("C-5 01 80 9000" == n0.represent());

  mt::NoteEvent n1{2};
  CHECK("--- -- -- ---- ----" == n1.represent());
  n1.note = mt::NoteDef{mt::Note::D_sharp, 4};
  n1.instr = 0;
  n1.volume = 0xFF;
  n1.mod[0] = 0x80FF;
  n1.mod[1] = 0x1090;
  CHECK("D#4 00 ff 80ff 1090" == n1.represent());

  mt::NoteEvent n2{1};
  n2.stop = true;
  CHECK("=== -- -- ----" == n2.represent());
}
