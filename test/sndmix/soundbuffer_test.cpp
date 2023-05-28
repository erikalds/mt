#include "sndmix/soundbuffer.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <fmt/format.h>
#include <chrono>
#include <cstdint>
#include <limits>
#include <span>

namespace Catch {
  template<>
  struct StringMaker<std::chrono::microseconds>
  {
    static std::string convert(const std::chrono::microseconds& x)
    {
      return fmt::format("{} us.", x.count());
    }
  };
}  // namespace Catch

namespace mt::snd::unittest {

  namespace {

    std::unique_ptr<std::int16_t[]> create_samples()
    {
      std::unique_ptr<std::int16_t[]> samples(new std::int16_t[8]);
      samples[0] = 0;
      samples[1] = 0;
      samples[2] = std::numeric_limits<std::int16_t>::max();          // 0.99997
      samples[3] = std::numeric_limits<std::int16_t>::min();          // -1.0
      samples[4] = std::numeric_limits<std::int16_t>::max() / 2 + 1;  // 0.5
      samples[5] = std::numeric_limits<std::int16_t>::min() / 2;      // -0.5
      samples[6] = std::numeric_limits<std::int16_t>::min();          // -1.0
      samples[7] = std::numeric_limits<std::int16_t>::max();          // 0.99997
      return samples;
    }

  }  // anonymous namespace

  TEST_CASE("simple_getters", "[sndmix]")
  {
    auto samples_owner = create_samples();
    std::int16_t* samples = samples_owner.get();
    SoundBuffer sndbuf{std::move(samples_owner), 4, 2, 44100};
    CHECK(sndbuf.get_sample_count() == 4);
    CHECK(sndbuf.get_sample_rate() == 44100);
    CHECK(sndbuf.get_channel_count() == 2);
    CHECK(sndbuf.get_samples() == samples);

    SoundBuffer sndbuf2{create_samples(), 8, 1, 22050};
    CHECK(sndbuf2.get_sample_count() == 8);
    CHECK(sndbuf2.get_sample_rate() == 22050);
    CHECK(sndbuf2.get_channel_count() == 1);
  }

  TEST_CASE("calculates_duration", "[sndmix]")
  {
    SoundBuffer sndbuf{create_samples(), 4, 2, 44100};
    // 4 * 1000000 / 44100 ~= 90.703 us
    CHECK(sndbuf.get_duration() == std::chrono::microseconds(91));
    SoundBuffer sndbuf2{create_samples(), 8, 1, 22050};
    // 8 * 1000000 / 22050 ~= 362.812 us
    CHECK(sndbuf2.get_duration() == std::chrono::microseconds(363));
  }

  TEST_CASE("converts_int16_t_data_to_float", "[sndmix]")
  {
    SoundBuffer sndbuf{create_samples(), 4, 2, 44100};
    REQUIRE(sndbuf.get_float_samples() != nullptr);
    std::span<const float> samples{sndbuf.get_float_samples(), 8};
    CHECK(samples[0] == Catch::Approx(0.0F));
    CHECK(samples[1] == Catch::Approx(0.0F));
    CHECK(samples[2] == Catch::Approx(0.99997F));
    CHECK(samples[3] == Catch::Approx(-1.0F));
    CHECK(samples[4] == Catch::Approx(0.5F));
    CHECK(samples[5] == Catch::Approx(-0.5F));
    CHECK(samples[6] == Catch::Approx(-1.0F));
    CHECK(samples[7] == Catch::Approx(0.99997F));
  }

  TEST_CASE("copy_ctor", "[sndmix]")
  {
    SoundBuffer sndbuf{create_samples(), 4, 2, 44100};
    const SoundBuffer copy = sndbuf; // NOLINT
    CHECK(sndbuf.get_sample_count() == copy.get_sample_count());
    CHECK(sndbuf.get_sample_rate() == copy.get_sample_rate());
    CHECK(sndbuf.get_duration() == copy.get_duration());
    CHECK(sndbuf.get_channel_count() == copy.get_channel_count());
    REQUIRE(copy.get_samples() != nullptr);
    CHECK(copy.get_samples() != sndbuf.get_samples());
    const auto n = copy.get_channel_count() * copy.get_sample_count();
    std::span<const std::int16_t> samples{copy.get_samples(), n};
    const auto* origsamples = sndbuf.get_samples();
    for (const auto& s : samples)
    {
      CHECK(s == *origsamples++);
    }
    std::span<const float> fsamples{copy.get_float_samples(), n};
    const auto* origfsamples = sndbuf.get_float_samples();
    for (const auto& s : fsamples)
    {
      CHECK(s == *origfsamples++);
    }
  }

  TEST_CASE("assignment_operator", "[sndmix]")
  {
    SoundBuffer sndbuf{create_samples(), 8, 1, 22050};
    SoundBuffer copy{};
    copy = sndbuf;
    CHECK(sndbuf.get_sample_count() == copy.get_sample_count());
    CHECK(sndbuf.get_sample_rate() == copy.get_sample_rate());
    CHECK(sndbuf.get_duration() == copy.get_duration());
    CHECK(sndbuf.get_channel_count() == copy.get_channel_count());
    REQUIRE(copy.get_samples() != nullptr);
    CHECK(copy.get_samples() != sndbuf.get_samples());
    const auto n = copy.get_channel_count() * copy.get_sample_count();
    std::span<const std::int16_t> samples{copy.get_samples(), n};
    const auto* origsamples = sndbuf.get_samples();
    for (const auto& s : samples)
    {
      CHECK(s == *origsamples++);
    }
    std::span<const float> fsamples{copy.get_float_samples(), n};
    const auto* origfsamples = sndbuf.get_float_samples();
    for (const auto& s : fsamples)
    {
      CHECK(s == *origfsamples++);
    }
  }
}  // namespace mt::snd::unittest
