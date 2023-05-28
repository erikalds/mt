#include "mtlib/pattern.h"

#include <catch2/catch_test_macros.hpp>
#include <yaml-cpp/yaml.h>

TEST_CASE("Pattern_Track_containers", "[pattern]")
{
  mt::Pattern p0{"name", 0x40, 8};
  mt::Pattern p1{"name", 0x60, 12};
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

TEST_CASE("NoteEvent_load_from_yaml", "[pattern]")
{
  YAML::Node node;
  mt::NoteEvent expected{1};
  auto n0 = mt::NoteEvent::load_from_yaml(node);
  CHECK(expected == n0);

  expected.stop = true;
  node["stop"] = true;
  n0 = mt::NoteEvent::load_from_yaml(node);
  CHECK(expected == n0);

  expected.stop = false;
  node["stop"] = false;
  expected.note = mt::NoteDef{mt::Note::C, 5};
  node["note"] = "C-5";
  n0 = mt::NoteEvent::load_from_yaml(node);
  CHECK(expected == n0);

  expected.instr = 4;
  node["instr"] = 4;
  n0 = mt::NoteEvent::load_from_yaml(node);
  CHECK(expected == n0);

  expected.volume = 42;
  node["volume"] = 42;
  n0 = mt::NoteEvent::load_from_yaml(node);
  CHECK(expected == n0);

  expected.mod[0] = 0x4567;
  node["mod"].push_back(0x4567);
  n0 = mt::NoteEvent::load_from_yaml(node);
  CHECK(expected == n0);
}

TEST_CASE("NoteEvent_get_as_yaml", "[pattern]")
{
  mt::NoteEvent ne{1};
  CHECK(!ne.get_as_yaml());

  ne.stop = true;
  auto node = *ne.get_as_yaml();
  CHECK(true == node["stop"].as<bool>());
  CHECK(!node["note"]);
  CHECK(!node["instr"]);
  CHECK(!node["volume"]);
  CHECK(!node["mod"]);

  ne.stop = false;
  ne.note = mt::NoteDef{mt::Note::F_sharp, 4};
  node = *ne.get_as_yaml();
  CHECK(!node["stop"]);
  CHECK("F#4" == node["note"].as<std::string>());
  CHECK(!node["instr"]);
  CHECK(!node["volume"]);
  CHECK(!node["mod"]);

  ne.instr = 4;
  node = *ne.get_as_yaml();
  CHECK(!node["stop"]);
  CHECK("F#4" == node["note"].as<std::string>());
  CHECK(4 == node["instr"].as<int>());
  CHECK(!node["volume"]);
  CHECK(!node["mod"]);

  ne.volume = 2;
  node = *ne.get_as_yaml();
  CHECK(!node["stop"]);
  CHECK("F#4" == node["note"].as<std::string>());
  CHECK(4 == node["instr"].as<int>());
  CHECK(2 == node["volume"].as<int>());
  CHECK(!node["mod"]);

  ne.mod[0] = 4242;
  node = *ne.get_as_yaml();
  CHECK(!node["stop"]);
  CHECK("F#4" == node["note"].as<std::string>());
  CHECK(4 == node["instr"].as<int>());
  CHECK(2 == node["volume"].as<int>());
  CHECK(4242 == node["mod"][0].as<int>());
}

TEST_CASE("Track_load_from_yaml", "[pattern]")
{
  YAML::Node node;
  node["length"] = 0x60;
  auto track = mt::Track::load_from_yaml(node);
  CHECK(0x60 == track.size());
  node["length"] = 0x40;
  track = mt::Track::load_from_yaml(node);
  CHECK(0x40 == track.size());

  node["events"][0]["note"] = "C#4";
  node["events"][0]["instr"] = 4;
  node["events"][0x10]["note"] = "C#3";
  node["events"][0x10]["instr"] = 5;
  track = mt::Track::load_from_yaml(node);

  mt::NoteEvent ne{1};
  CHECK(ne == track[1]);
  CHECK(ne == track[2]);

  ne.note = mt::NoteDef{mt::Note::C_sharp, 4};
  ne.instr = 4;
  CHECK(ne == track[0]);

  ne.note = mt::NoteDef{mt::Note::C_sharp, 3};
  ne.instr = 5;
  CHECK(ne == track[0x10]);
}

TEST_CASE("Track_get_as_yaml", "[pattern]")
{
  mt::Track t0{0x40};
  mt::Track t1{0x60};
  t0[0].note = mt::NoteDef{mt::Note::A_sharp, 4};
  t0[0].instr = 4;
  t0[8].note = mt::NoteDef{mt::Note::C_sharp, 5};
  t0[8].instr = 5;
  t1[4].note = mt::NoteDef{mt::Note::A, 3};
  t1[4].instr = 1;
  t1[12].note = mt::NoteDef{mt::Note::C, 6};
  t1[12].instr = 0;

  auto n0 = t0.get_as_yaml();
  auto n1 = t1.get_as_yaml();
  CHECK(n0["length"].as<int>() == 0x40);
  CHECK(n0["events"][0]["note"].as<std::string>() == "A#4");
  CHECK(n0["events"][0]["instr"].as<int>() == 4);
  CHECK(!n0["events"][4]);
  CHECK(n0["events"][8]["note"].as<std::string>() == "C#5");
  CHECK(n0["events"][8]["instr"].as<int>() == 5);

  CHECK(n1["length"].as<int>() == 0x60);
  CHECK(!n1["events"][0]);
  CHECK(n1["events"][4]["note"].as<std::string>() == "A-3");
  CHECK(n1["events"][4]["instr"].as<int>() == 1);
  CHECK(n1["events"][12]["note"].as<std::string>() == "C-6");
  CHECK(n1["events"][12]["instr"].as<int>() == 0);
}

TEST_CASE("Pattern_load_from_yaml", "[pattern]")
{
  YAML::Node node;
  node["name"] = "pattern_name";
  node["track_count"] = 4;
  mt::Track t0{0x60};
  t0[0].note = mt::NoteDef{mt::Note::D, 4};
  t0[0].instr = 4;
  mt::Track t1{0x60};
  t1[4].note = mt::NoteDef{mt::Note::E, 5};
  t1[4].instr = 3;
  mt::Track t2{0x60};
  t2[8].note = mt::NoteDef{mt::Note::F, 3};
  t2[8].instr = 2;
  mt::Track t3{0x60};
  t3[12].note = mt::NoteDef{mt::Note::G, 2};
  t3[12].instr = 1;
  node["tracks"][0] = t0.get_as_yaml();
  node["tracks"][1] = t1.get_as_yaml();
  node["tracks"][2] = t2.get_as_yaml();
  node["tracks"][3] = t3.get_as_yaml();

  auto pattern = mt::Pattern::load_from_yaml(node);
  CHECK(4 == pattern->size());
  CHECK("pattern_name" == pattern->name());
  CHECK(0x60 == pattern->get_track_length());
  CHECK(t0[0] == (*pattern)[0][0]);
  CHECK(t1[4] == (*pattern)[1][4]);
  CHECK(t2[8] == (*pattern)[2][8]);
  CHECK(t3[12] == (*pattern)[3][12]);
}

TEST_CASE("Pattern_get_as_yaml", "[pattern]")
{
  mt::Pattern p{"cool", 0x60, 8};
  p[0][0].note = mt::NoteDef{mt::Note::D, 4};
  p[0][0].instr = 4;
  p[1][4].note = mt::NoteDef{mt::Note::E, 5};
  p[1][4].instr = 3;
  p[2][8].note = mt::NoteDef{mt::Note::F, 3};
  p[2][8].instr = 2;
  p[3][12].note = mt::NoteDef{mt::Note::G, 2};
  p[3][12].instr = 1;

  auto node = p.get_as_yaml();
  CHECK("cool" == node["name"].as<std::string>());
  CHECK(8 == node["track_count"].as<int>());
  CHECK("D-4" == node["tracks"][0]["events"][0]["note"].as<std::string>());
  CHECK(4 == node["tracks"][0]["events"][0]["instr"].as<int>());
  CHECK("E-5" == node["tracks"][1]["events"][4]["note"].as<std::string>());
  CHECK(3 == node["tracks"][1]["events"][4]["instr"].as<int>());
  CHECK("F-3" == node["tracks"][2]["events"][8]["note"].as<std::string>());
  CHECK(2 == node["tracks"][2]["events"][8]["instr"].as<int>());
  CHECK("G-2" == node["tracks"][3]["events"][12]["note"].as<std::string>());
  CHECK(1 == node["tracks"][3]["events"][12]["instr"].as<int>());
}
