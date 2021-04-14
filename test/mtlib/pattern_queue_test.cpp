#include "mtlib/pattern_queue.h"

#include "mtlib/pattern.h"
#include <catch2/catch.hpp>
#include <yaml-cpp/yaml.h>


namespace {

  bool all_note_events_noop(const mt::Pattern& p)
  {
    for (const auto& track : p)
    {
      for (const auto& noteevent : track)
      {
        if (noteevent.stop
            || noteevent.note
            || noteevent.instr
            || noteevent.volume
            || noteevent.mod.size() != 1U
            || noteevent.mod.at(0))
        {
          return false;
        }
      }
    }
    return true;
  }

} // anonymous namespace

TEST_CASE("PatternQueue_container_default_ctor", "[pattern]")
{
  mt::PatternQueue pq;
  SECTION("at_and_bracket_operator")
  {
    CHECK(1U == pq.size());
    CHECK(all_note_events_noop(pq.at(0)));
    CHECK(all_note_events_noop(pq[0]));
  }
  SECTION("is a range")
  {
    CHECK(std::distance(std::begin(pq), std::end(pq)) == 1U);
    for (const mt::Pattern& p : pq)
    {
      CHECK(p.size() == 8);
      CHECK(p.get_track_length() == 0x40);
    }
  }
}

TEST_CASE("PatternQueue_sets_name_of_pattern", "[pattern]")
{
  mt::PatternQueue pq;
  CHECK("00" == pq.at(0).name());
  pq.at(0).set_name("name");
  CHECK("name" == pq.at(0).name());
}

TEST_CASE("PatternQueue_append_duplicate_pattern", "[pattern]")
{
  mt::PatternQueue pq;
  auto iter = std::begin(pq);
  pq.append_duplicate_pattern(iter);
  CHECK(2U == pq.size());
  ++iter;
  CHECK(&*std::begin(pq) == &*(iter));
  CHECK(&pq[0] == &pq.at(1));

  pq.append_duplicate_pattern(iter);
  CHECK(3U == pq.size());
  CHECK(&pq[0] == &pq.at(2));

  ++iter;
  pq.increment_pattern_at(iter);
  CHECK(3U == pq.size());
  CHECK(&pq[0] != &pq.at(2));
  pq.append_duplicate_pattern(iter);
  CHECK(4U == pq.size());
  CHECK(&pq[0] != &pq.at(3));
  CHECK(pq[0].name() != pq.at(3).name());
  CHECK(&pq[2] == &pq.at(3));
  CHECK(pq[2].name() == pq.at(3).name());
}

TEST_CASE("PatternQueue_remove_pattern_from_queue", "[pattern]")
{
  mt::PatternQueue pq;
  auto iter = std::begin(pq);
  pq.append_duplicate_pattern(iter);
  ++iter;
  pq.increment_pattern_at(iter);
  pq.append_duplicate_pattern(iter);
  ++iter;
  pq.increment_pattern_at(iter);
  pq.append_duplicate_pattern(iter);
  // 0: 00, 1: 01, 2: 02, 3: 02
  auto iter2 = ++std::begin(pq);
  pq.remove_pattern_from_queue(iter2);
  CHECK(3U == pq.size());
  CHECK("00" == pq[0].name());
  CHECK("02" == pq[1].name());
  CHECK("02" == pq[2].name());
  pq.remove_pattern_from_queue(iter2);
  CHECK(2U == pq.size());
  CHECK("00" == pq[0].name());
  CHECK("02" == pq[1].name());
  --iter2;
  pq.remove_pattern_from_queue(iter2);
  CHECK(1U == pq.size());
  CHECK("02" == pq[0].name());
  // cannot remove the only pattern of a pattern queue
  pq.remove_pattern_from_queue(iter2);
  CHECK(1U == pq.size());
  CHECK("02" == pq[0].name());
}

TEST_CASE("PatternQueue_increment_pattern_at", "[pattern]")
{
  mt::PatternQueue pq;
  pq.append_duplicate_pattern(std::begin(pq));
  pq.append_duplicate_pattern(std::begin(pq));
  CHECK(3U == pq.size());
  pq.increment_pattern_at(++std::begin(pq));
  CHECK("00" == pq[0].name());
  CHECK("01" == pq[1].name());
  CHECK("00" == pq.at(2).name());
  CHECK(&pq[0] == &pq.at(2));
  CHECK(&pq.at(0) != &pq[1]);
}

TEST_CASE("PatternQueue_decrement_pattern_at", "[pattern]")
{
  mt::PatternQueue pq;
  auto iter = std::begin(pq);
  pq.append_duplicate_pattern(iter);
  pq.append_duplicate_pattern(iter);
  CHECK(3U == pq.size());
  ++iter;
  pq.increment_pattern_at(iter);
  ++iter;
  pq.increment_pattern_at(iter);
  pq.increment_pattern_at(iter);
  CHECK("00" == pq[0].name());
  CHECK("01" == pq[1].name());
  CHECK("02" == pq.at(2).name());
  CHECK(&pq[0] != &pq.at(2));
  CHECK(&pq.at(0) != &pq[1]);
  pq.decrement_pattern_at(iter);
  --iter;
  pq.decrement_pattern_at(iter);
  // cannot decrement past 0, stay at 0
  --iter;
  pq.decrement_pattern_at(iter);
  CHECK("00" == pq[0].name());
  CHECK("00" == pq[1].name());
  CHECK("01" == pq.at(2).name());
  CHECK(&pq[0] != &pq.at(2));
  CHECK(&pq.at(0) == &pq[1]);
}

TEST_CASE("PatternQueue_iterator", "[pattern]")
{
  mt::PatternQueue pq;
  pq.append_duplicate_pattern(std::begin(pq));
  pq.append_duplicate_pattern(std::begin(pq));
  pq.increment_pattern_at(++std::begin(pq));
  pq.increment_pattern_at(++++std::begin(pq));
  pq.increment_pattern_at(++++std::begin(pq));
  const auto data_size = pq.size();
  const auto sdata_size = static_cast<ssize_t>(data_size);

  // Situation is now: 00: 00, 01: 01, 02: 02
  mt::PatternQueue::iterator iter = pq.begin();
  mt::PatternQueue::iterator end = pq.end();
  mt::PatternQueue::const_iterator citer = pq.begin();
  mt::PatternQueue::const_iterator cend = pq.end();

  SECTION("equality")
  {
    CHECK(iter != end);
    CHECK(citer != cend);
    CHECK(end != iter);
    CHECK(cend != citer);
    CHECK(iter == iter);
    CHECK(citer == citer);
    CHECK(end == end);
    CHECK(cend == cend);
  }
  SECTION("iterate forward")
  {
    auto i = 0U;
    for (; iter != end && citer != cend; ++iter, ++citer)
    {
      REQUIRE(i < data_size);
      auto expected_name = fmt::format("{:02x}", i++);
      CHECK(expected_name == iter->name());
      CHECK(expected_name == citer->name());
    }
    CHECK(iter == end);
    CHECK(citer == cend);
  }
  SECTION("iterate backwards")
  {
    auto begin = iter;
    auto cbegin = citer;
    CHECK("00" == (*iter++).name());
    CHECK("00" == (*citer++).name());
    CHECK("02" == (*++iter).name());
    CHECK("02" == (*++citer).name());
    CHECK("01" == (*--iter).name());
    CHECK("01" == (*--citer).name());
    CHECK(begin != iter);
    CHECK(cbegin != citer);
    CHECK("01" == (*iter--).name());
    CHECK("01" == (*citer--).name());
    CHECK(begin == iter);
    CHECK(cbegin == citer);
    CHECK_THROWS_AS(--iter, std::logic_error);
    CHECK_THROWS_AS(--citer, std::logic_error);
    CHECK_THROWS_AS(iter--, std::logic_error);
    CHECK_THROWS_AS(citer--, std::logic_error);
  }
  SECTION("iterate in steps")
  {
    pq.append_duplicate_pattern(--std::end(pq));
    pq.increment_pattern_at(--std::end(pq));
    pq.append_duplicate_pattern(--std::end(pq));
    pq.increment_pattern_at(--std::end(pq));
    iter += 4;
    citer += 4;
    CHECK("04" == iter->name());
    CHECK("04" == citer->name());
    iter -= 2;
    citer -= 3;
    CHECK("02" == iter->name());
    CHECK("01" == citer->name());
    iter -= 2;
    citer -= 1;
    CHECK("00" == iter->name());
    CHECK("00" == citer->name());
    CHECK_THROWS_AS(iter -= 2, std::logic_error);
    CHECK_THROWS_AS(citer -= 2, std::logic_error);
    CHECK_THROWS_AS(iter += -2, std::logic_error);
    CHECK_THROWS_AS(citer += -2, std::logic_error);

    CHECK("04" == (iter + 4)->name());
    CHECK("03" == (citer + 3)->name());
    CHECK("03" == ((4 + iter) - 1)->name());
    CHECK("01" == ((3 + citer) - 2)->name());
  }

  SECTION("bracket operator")
  {
    CHECK("01" == iter[1].name());
    CHECK("01" == citer[1].name());
    ++iter;
    ++citer;
    CHECK("02" == iter[1].name());
    CHECK("02" == iter[1].name());
  }

  SECTION("comparable")
  {
    CHECK(iter < (iter + 1));
    CHECK(citer < (citer + 1));
    CHECK(!(iter < iter));
    CHECK(!(citer < citer));
    CHECK(iter <= (iter + 1));
    CHECK(citer <= (citer + 1));
    CHECK(iter <= iter);
    CHECK(citer <= citer);

    CHECK((iter + 1) > iter);
    CHECK((citer + 1) > citer);
    CHECK(!(iter > iter));
    CHECK(!(citer > citer));
    CHECK((iter + 1) >= iter);
    CHECK((citer + 1) >= citer);
    CHECK(iter >= iter);
    CHECK(citer >= citer);

    CHECK(iter < end);
    CHECK(citer < cend);
    CHECK(!(end < end));
    CHECK(!(cend < cend));
    CHECK(iter <= end);
    CHECK(citer <= cend);
    CHECK(end <= end);
    CHECK(cend <= cend);
    CHECK(!((iter + sdata_size) < end));
    CHECK(!((citer + sdata_size) < cend));
    CHECK((iter + sdata_size) <= end);
    CHECK((citer + sdata_size) <= cend);

    CHECK(end > iter);
    CHECK(cend > citer);
    CHECK(!(end > end));
    CHECK(!(cend > cend));
    CHECK(end >= iter);
    CHECK(cend >= citer);
    CHECK(end >= end);
    CHECK(cend >= cend);
    CHECK(!(end > (iter + sdata_size)));
    CHECK(!(cend > (citer + sdata_size)));
    CHECK(end >= (iter + sdata_size));
    CHECK(cend >= (citer + sdata_size));
  }
  SECTION("distance")
  {
    CHECK(3 == std::distance(iter, end));
    CHECK(3 == std::distance(citer, cend));
    CHECK(1 == std::distance(iter + 1, iter + 2));
    CHECK(-1 == std::distance(iter + 2, iter + 1));
    CHECK(2 == std::distance(citer, citer + 2));
    CHECK(-2 == std::distance(citer + 2, citer));
    CHECK(0 == std::distance(end, end));
    CHECK(0 == std::distance(cend, cend));
  }
}

TEST_CASE("PatternQueue_load_from_yaml", "[pattern]")
{
  YAML::Node node{};
  mt::Pattern p0{"p0", 0x40, 8};
  node["patterns"][0] = p0.get_as_yaml();
  mt::Pattern p1{"p1", 0x50, 8};
  node["patterns"][1] = p1.get_as_yaml();
  mt::Pattern p2{"p2", 0x60, 8};
  node["patterns"][2] = p2.get_as_yaml();
  node["queue"][0] = 0;
  node["queue"][1] = 0;
  node["queue"][2] = 0;
  node["queue"][3] = 1;
  node["queue"][4] = 2;
  node["queue"][5] = 2;
  node["queue"][6] = 2;
  node["queue"][7] = 1;

  auto pq = mt::PatternQueue::load_from_yaml(node);
  CHECK("p0" == pq->at(0).name());
  CHECK("p0" == pq->at(1).name());
  CHECK("p0" == pq->at(2).name());
  CHECK("p1" == pq->at(3).name());
  CHECK("p2" == pq->at(4).name());
  CHECK("p2" == pq->at(5).name());
  CHECK("p2" == pq->at(6).name());
  CHECK("p1" == pq->at(7).name());
}

TEST_CASE("PatternQueue_get_as_yaml", "[pattern]")
{
  mt::PatternQueue pq{};
  pq[0].set_name("intro");
  pq.append_duplicate_pattern(pq.begin());
  pq.increment_pattern_at(++pq.begin());
  pq[1].set_name("body");
  pq.append_duplicate_pattern(++pq.begin());
  pq.append_duplicate_pattern(++pq.begin());
  pq.append_duplicate_pattern(++pq.begin());
  pq.append_duplicate_pattern(++pq.begin());
  pq.increment_pattern_at(--pq.end());
  pq[5].set_name("outro");

  auto node = pq.get_as_yaml();
  CHECK("intro" == node["patterns"][0]["name"].as<std::string>());
  CHECK("body" == node["patterns"][1]["name"].as<std::string>());
  CHECK("outro" == node["patterns"][2]["name"].as<std::string>());
  CHECK(0 == node["queue"][0].as<int>());
  CHECK(1 == node["queue"][1].as<int>());
  CHECK(1 == node["queue"][2].as<int>());
  CHECK(1 == node["queue"][3].as<int>());
  CHECK(1 == node["queue"][4].as<int>());
  CHECK(2 == node["queue"][5].as<int>());
}
