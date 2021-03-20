#include "mtlib/sample_data_iterator.h"

#include <catch2/catch.hpp>
#include <cstdint>

TEST_CASE("sample_data_iterator.mono_sample", "[iterator]")
{
  constexpr const auto data_size = 10;
  std::array<std::int16_t, data_size> data{};
  for (auto i = 0U; i < data_size; ++i)
  {
    data.at(i) = static_cast<std::int16_t>(i);
  }
  mt::sample_data_iterator<std::int16_t> iter{&data[0], data_size};
  mt::sample_data_iterator<std::int16_t> end{};

  SECTION("equality")
  {
    CHECK(iter != end);
    CHECK(end == end);
    CHECK(iter == iter);
  }
  SECTION("iterate forward")
  {
    std::int16_t i = 0;
    for (; iter != end; ++iter)
    {
      REQUIRE(i < data_size);
      CHECK(i++ == *iter);
    }
    CHECK(iter == end);
  }
  SECTION("iterate backwards")
  {
    auto begin = iter;
    CHECK(0 == *iter++);
    CHECK(2 == *++iter);
    CHECK(1 == *--iter);
    CHECK(begin != iter);
    CHECK(1 == *iter--);
    CHECK(begin == iter);
    CHECK_THROWS_AS(--iter, std::logic_error);
    CHECK_THROWS_AS(iter--, std::logic_error);
  }
  SECTION("iterate in steps")
  {
    iter += 4;
    CHECK(4 == *iter);
    iter -= 2;
    CHECK(2 == *iter);
    iter -= 2;
    CHECK(0 == *iter);
    CHECK_THROWS_AS(iter -= 2, std::logic_error);

    CHECK(5 == *(iter + 5));
    CHECK(3 == *((5 + iter) - 2));
  }
  SECTION("bracket operator")
  {
    CHECK(4 == iter[4]);
    ++iter;
    CHECK(7 == iter[6]);
  }
  SECTION("comparable")
  {
    CHECK(iter < (iter + 1));
    CHECK(!(iter < iter));
    CHECK(iter <= (iter + 1));
    CHECK(iter <= iter);

    CHECK((iter + 1) > iter);
    CHECK(!(iter > iter));
    CHECK((iter + 1) >= iter);
    CHECK(iter >= iter);

    CHECK(iter < end);
    CHECK(!(end < end));
    CHECK(iter <= end);
    CHECK(end <= end);
    CHECK(!((iter + data.size()) < end));
    CHECK((iter + data.size()) <= end);

    CHECK(end > iter);
    CHECK(!(end > end));
    CHECK(end >= iter);
    CHECK(end >= end);
    CHECK(!(end > (iter + data.size())));
    CHECK(end >= (iter + data.size()));
  }
  SECTION("distance")
  {
    CHECK(10U == std::distance(iter, end));
    CHECK(4U == std::distance(iter + 2, iter + 6));
    CHECK(0U == std::distance(end, end));
  }
}

TEST_CASE("sample_data_iterator.stereo_sample", "[iterator]")
{
  constexpr const auto data_size = 20;
  std::array<std::int16_t, data_size> data{};
  constexpr const auto sample_size = data_size / 2;
  for (auto i = 0U; i < sample_size; ++i)
  {
    data.at(2 * i) = static_cast<std::int16_t>(i);
    data.at(2 * i + 1) = static_cast<std::int16_t>(sample_size + i);
  }

  mt::sample_data_iterator<std::int16_t> liter{&data[0], data_size, 2};
  mt::sample_data_iterator<std::int16_t> riter{&data[0], data_size, 2, 1};
  mt::sample_data_iterator<std::int16_t> end{};

  SECTION("equality")
  {
    CHECK(liter != end);
    CHECK(end == end);
    CHECK(riter == riter);
  }
  SECTION("iterate forward")
  {
    std::int16_t i = 0;
    for (; liter != end; ++liter)
    {
      REQUIRE(i < sample_size);
      CHECK(i++ == *liter);
    }
    CHECK(liter == end);
    for (; riter != end; ++riter)
    {
      REQUIRE(i < data_size);
      CHECK(i++ == *riter);
    }
    CHECK(riter == end);
  }
  SECTION("iterate backwards")
  {
    auto lbegin = liter;
    CHECK(0 == *liter++);
    CHECK(2 == *++liter);
    CHECK(1 == *--liter);
    CHECK(lbegin != liter);
    CHECK(1 == *liter--);
    CHECK(lbegin == liter);
    CHECK_THROWS_AS(--liter, std::logic_error);
    CHECK_THROWS_AS(liter--, std::logic_error);
    auto rbegin = riter;
    CHECK(10 == *riter++);
    CHECK(12 == *++riter);
    CHECK(11 == *--riter);
    CHECK(rbegin != riter);
    CHECK(11 == *riter--);
    CHECK(rbegin == riter);
    CHECK_THROWS_AS(--riter, std::logic_error);
    CHECK_THROWS_AS(riter--, std::logic_error);
  }
  SECTION("iterate in steps")
  {
    liter += 4;
    CHECK(4 == *liter);
    liter -= 2;
    CHECK(2 == *liter);
    liter -= 2;
    CHECK(0 == *liter);
    CHECK_THROWS_AS(liter -= 2, std::logic_error);

    CHECK(5 == *(liter + 5));
    CHECK(3 == *((5 + liter) - 2));

    riter += 4;
    CHECK(14 == *riter);
    riter -= 2;
    CHECK(12 == *riter);
    riter -= 2;
    CHECK(10 == *riter);
    CHECK_THROWS_AS(riter -= 2, std::logic_error);

    CHECK(15 == *(riter + 5));
    CHECK(13 == *((5 + riter) - 2));
  }
  SECTION("bracket operator")
  {
    CHECK(4 == liter[4]);
    ++liter;
    CHECK(7 == liter[6]);
    CHECK(14 == riter[4]);
    ++riter;
    CHECK(17 == riter[6]);
  }
  SECTION("comparable")
  {
    CHECK(liter < (liter + 1));
    CHECK(!(liter < liter));
    CHECK(liter <= (liter + 1));
    CHECK(liter <= liter);

    CHECK((liter + 1) > liter);
    CHECK(!(liter > liter));
    CHECK((liter + 1) >= liter);
    CHECK(liter >= liter);

    CHECK(liter < end);
    CHECK(!(end < end));
    CHECK(liter <= end);
    CHECK(end <= end);
    CHECK(!((liter + data.size()) < end));
    CHECK((liter + data.size()) <= end);

    CHECK(end > liter);
    CHECK(!(end > end));
    CHECK(end >= liter);
    CHECK(end >= end);
    CHECK(!(end > (liter + data.size())));
    CHECK(end >= (liter + data.size()));


    CHECK(riter < (riter + 1));
    CHECK(!(riter < riter));
    CHECK(riter <= (riter + 1));
    CHECK(riter <= riter);

    CHECK((riter + 1) > riter);
    CHECK(!(riter > riter));
    CHECK((riter + 1) >= riter);
    CHECK(riter >= riter);

    CHECK(riter < end);
    CHECK(!(end < end));
    CHECK(riter <= end);
    CHECK(end <= end);
    CHECK(!((riter + data.size()) < end));
    CHECK((riter + data.size()) <= end);

    CHECK(end > riter);
    CHECK(!(end > end));
    CHECK(end >= riter);
    CHECK(end >= end);
    CHECK(!(end > (riter + data.size())));
    CHECK(end >= (riter + data.size()));
  }
  SECTION("distance")
  {
    CHECK(10U == std::distance(liter, end));
    CHECK(10U == std::distance(riter, end));
    CHECK(9U == std::distance(riter + 1, end));
    CHECK(4U == std::distance(liter + 3, liter + 7));
    CHECK(5U == std::distance(riter + 3, riter + 8));
    CHECK(4U == std::distance(riter + 4, riter + 8));
  }
}
