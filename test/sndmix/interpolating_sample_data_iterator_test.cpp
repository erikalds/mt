#include "sndmix/interpolating_sample_data_iterator.h"
#include "sndmix/sample_data_iterator.h"
#include <catch2/catch.hpp>

#include <iostream>

namespace mt::snd::unittest {

TEST_CASE("interpolate_22050_to_44100", "[iterator]")
{
  constexpr const auto data_size = 10;
  std::array<float, data_size> data{};
  std::array<float, 2 * data_size - 1> correct_output{};
  for (auto i = 0U; i < data_size; ++i)
  {
    data.at(i) = static_cast<float>(i) / data_size;
    correct_output.at(i * 2) = static_cast<float>(i * 2) / (data_size * 2);
    // we cannot interpolate past the end of data
    if (i * 2 + 1 < correct_output.size())
    {
      correct_output.at(i * 2 + 1) = static_cast<float>(i * 2 + 1) / (data_size * 2);
    }
  }

  sample_data_iterator<float> iiter{&data[0], data_size};
  sample_data_iterator<float> iend{};

  const auto input_sample_rate{22050};
  const auto output_sample_rate{44100};
  interpolating_sample_data_iterator<float> iter{iiter, iend, input_sample_rate, output_sample_rate};
  interpolating_sample_data_iterator<float> end{};

  SECTION("equality")
  {
    CHECK(iter != end);
    CHECK(end == end);
    CHECK(iter == iter);
    auto iter2 = iter;
    ++iter2;
    CHECK(iter2 != iter);
    ++iter;
    CHECK(iter2 == iter);
  }
  SECTION("iterate forward")
  {
    std::size_t i = 0;
    for (; iter != end; ++iter)
    {
      REQUIRE(i < correct_output.size());
      CHECK(Approx(correct_output.at(i++)) == *iter);
    }
    CHECK(iter == end);
    CHECK(i == correct_output.size());
  }
  SECTION("iterate backwards")
  {
    auto begin = iter;
    CHECK(correct_output[0] == *iter++);
    CHECK(correct_output[2] == *++iter);
    CHECK(correct_output[1] == *--iter);
    CHECK(begin != iter);
    CHECK(correct_output[1] == *iter--);
    CHECK(begin == iter);
    CHECK_THROWS_AS(--iter, std::logic_error);
    CHECK_THROWS_AS(iter--, std::logic_error);
  }
  SECTION("iterate in steps")
  {
    iter += 4;
    CHECK(correct_output[4] == *iter);
    iter -= 2;
    CHECK(correct_output[2] == *iter);
    iter -= 2;
    CHECK(correct_output[0] == *iter);
    CHECK_THROWS_AS(iter -= 2, std::logic_error);

    CHECK(correct_output[5] == *(iter + 5));
    CHECK(correct_output[3] == *((5 + iter) - 2));
  }
  SECTION("bracket operator")
  {
    CHECK(Approx(correct_output[4]) == iter[4]);
    ++iter;
    CHECK(Approx(correct_output[7]) == iter[6]);
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
    CHECK(!((iter + correct_output.size()) < end));
    CHECK((iter + correct_output.size()) <= end);

    CHECK(end > iter);
    CHECK(!(end > end));
    CHECK(end >= iter);
    CHECK(end >= end);
    CHECK(!(end > (iter + correct_output.size())));
    CHECK(end >= (iter + correct_output.size()));
  }
  SECTION("distance")
  {
    CHECK(correct_output.size() == std::distance(iter, end));
    CHECK(correct_output.size() - 3 == std::distance(iter + 3, end));
    CHECK(4U == std::distance(iter + 2, iter + 6));
    CHECK(4U == std::distance(iter + 6, iter + 2));
    CHECK(0U == std::distance(end, end));
  }
}

namespace {

  inline std::size_t lower_index(double fpv)
  {
    return static_cast<std::size_t>(std::floor(fpv));
  }

  inline std::size_t upper_index(double fpv)
  {
    return static_cast<std::size_t>(std::ceil(fpv));
  }

}  // anonymous namespace

TEST_CASE("interpolate_22050_to_48000", "[iterator]")
{
  const auto data_size{10U};
  const auto input_sample_rate{22050U};
  const auto output_sample_rate{48000U};
  std::array<float, data_size> data{};
  std::array<float, data_size * output_sample_rate / input_sample_rate - 1> correct_output{};
  for (auto i = 0U; i < data_size; ++i)
  {
    data.at(i) = static_cast<float>(i) / data_size;
  }
  const auto c = static_cast<double>(output_sample_rate) / static_cast<double>(input_sample_rate);
  for (auto i = 0U; i < correct_output.size(); ++i)
  {
    auto cv = i / c;
    auto k = cv - std::floor(cv);
    correct_output.at(i)
      = static_cast<float>((1.0 - k) * static_cast<double>(data.at(lower_index(cv)))
                           + k * static_cast<double>(data.at(upper_index(cv))));
  }

  sample_data_iterator<float> iiter{&data[0], data_size};
  sample_data_iterator<float> iend{};

  interpolating_sample_data_iterator<float> iter{iiter, iend, input_sample_rate, output_sample_rate};
  interpolating_sample_data_iterator<float> end{};
  for (auto i = 0U; i < correct_output.size() && iter != end; ++i, ++iter)
  {
    CHECK(*iter == correct_output.at(i));
  }
  CHECK(iter == end);
}

}  // namespace mt::snd::unittest
