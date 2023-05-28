#include "sndmix/sound.h"
#include "sndmix/soundbuffer.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <initializer_list>

namespace mt::snd::unittest {

namespace {

  void fill(std::int16_t* b, std::initializer_list<std::int16_t> il)
  {
    for (auto i = 0U; i < il.size(); ++i)
    {
      b[i] = *(il.begin() + i);
    }
  }

  std::vector<float>
  expected_output(std::size_t start, std::size_t end,
                  const std::function<float(std::size_t)>& generate)
  {
    std::vector<float> v;
    v.reserve(end - start);
    for (auto i = start; i < end; ++i)
    {
      v.push_back(generate(i));
    }
    return v;
  }

  constexpr float fp_sample(std::int16_t v)
  {
    const float max_16bit{32768.0};
    return static_cast<float>(v) / max_16bit;
  }
}

TEST_CASE("writes_all_mono_data_to_stereo_output", "[sndmix]")
{
  std::unique_ptr<std::int16_t[]> b{new std::int16_t[16]};
  fill(b.get(), { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
  const auto sample_rate{44100U};
  SoundBuffer sb{std::move(b), 16, 1, sample_rate};
  Sound sound{sb, 1.0};
  std::vector<float> output(34, 0.0F);
  std::span<float> ospan{&output[0], output.size()};
  auto opt_offset = sound.write_to(ospan, 2, sample_rate, 0);
  CHECK(!opt_offset);
  CHECK(output == expected_output(0, output.size(),
                                  [](std::size_t i)
                                  {
                                    if (i >= 32)
                                    {
                                      return 0.0F;
                                    }
                                    return fp_sample(static_cast<std::int16_t>(i) / 2);
                                  }));
}

TEST_CASE("writes_all_stereo_data_to_stereo_output", "[sndmix]")
{
  std::unique_ptr<std::int16_t[]> b{new std::int16_t[32]};
  fill(b.get(),
       { 0, 14, 2, 12, 4, 10, 6, 8, 8, 6, 10, 4, 12, 2, 14, 0,
         16, -2, 18, -4, 20, -6, 22, -8, 24, -10, 26, -12, 28, -14, 30, -16});
  const auto* data = b.get();
  const auto sample_rate{44100U};
  SoundBuffer sb{std::move(b), 16, 2, sample_rate};
  Sound sound{sb, 1.0};
  std::vector<float> output(34, 0.0F);
  std::span<float> ospan{&output[0], output.size()};
  auto opt_offset = sound.write_to(ospan, 2, sample_rate, 0);
  CHECK(!opt_offset);
  CHECK(output == expected_output(0, output.size(),
                                  [&data](std::size_t i)
                                  {
                                    if (i >= 32)
                                    {
                                      return 0.0F;
                                    }
                                    return fp_sample(data[i]);
                                  }));
}

TEST_CASE("writes_all_stereo_data_to_mono_output", "[sndmix]")
{
  std::unique_ptr<std::int16_t[]> b{new std::int16_t[32]};
  fill(b.get(),
       { 0, 14, 2, 12, 4, 10, 6, 8, 8, 6, 10, 4, 12, 2, 14, 0,
         16, -2, 18, -4, 20, -6, 22, -8, 24, -10, 26, -12, 28, -14, 30, -16});
  const auto* data = b.get();
  const auto sample_rate{44100U};
  SoundBuffer sb{std::move(b), 16, 2, sample_rate};
  Sound sound{sb, 1.0};
  std::vector<float> output(17, 0.0F);
  std::span<float> ospan{&output[0], output.size()};
  auto opt_offset = sound.write_to(ospan, 1, sample_rate, 0);
  CHECK(!opt_offset);
  CHECK(output == expected_output(0, output.size(),
                                  [&data](std::size_t i)
                                  {
                                    if (i >= 16)
                                    {
                                      return 0.0F;
                                    }
                                    return (fp_sample(data[2 * i])
                                            + fp_sample(data[2 * i + 1])) / 2.0F;
                                  }));
}

TEST_CASE("writes_stereo_data_to_stereo_output_in_two_batches", "[sndmix]")
{
  std::unique_ptr<std::int16_t[]> b{new std::int16_t[32]};
  fill(b.get(),
       { 0, 14, 2, 12, 4, 10, 6, 8, 8, 6, 10, 4, 12, 2, 14, 0,
         16, -2, 18, -4, 20, -6, 22, -8, 24, -10, 26, -12, 28, -14, 30, -16});
  const auto* data = b.get();
  const auto sample_rate{44100U};
  SoundBuffer sb{std::move(b), 16, 2, sample_rate};
  Sound sound{sb, 1.0};
  const auto bufsize = 18;
  std::vector<float> output0(bufsize, 0.0F);
  std::span<float> ospan0{&output0[0], output0.size()};
  auto opt_offset = sound.write_to(ospan0, 2, sample_rate, 0);
  CHECK(static_cast<bool>(opt_offset));
  CHECK(*opt_offset == bufsize);
  CHECK(output0 == expected_output(0, bufsize,
                                   [&data](std::size_t i)
                                   {
                                     return fp_sample(data[i]);
                                   }));

  std::vector<float> output1(bufsize, 0.0F);
  std::span<float> ospan1{&output1[0], output1.size()};
  opt_offset = sound.write_to(ospan1, 2, sample_rate, *opt_offset);
  CHECK(!opt_offset);
  CHECK(output1 == expected_output(bufsize, 2 * bufsize,
                                   [&data](std::size_t i)
                                   {
                                     if (i >= 32)
                                     {
                                       return 0.0F;
                                     }
                                     return fp_sample(data[i]);
                                   }));
}

TEST_CASE("writes_mono_data_to_stereo_output_in_two_batches", "[sndmix]")
{
  std::unique_ptr<std::int16_t[]> b{new std::int16_t[16]};
  fill(b.get(),
       { 0, 14, 2, 12, 4, 10, 6, 8, 8, 6, 10, 4, 12, 2, 14, 0});
  const auto* data = b.get();
  const auto sample_rate{44100U};
  SoundBuffer sb{std::move(b), 16, 1, sample_rate};
  Sound sound{sb, 1.0};
  const auto bufsize = 18;
  std::vector<float> output0(bufsize, 0.0F);
  std::span<float> ospan0{&output0[0], output0.size()};
  auto opt_offset = sound.write_to(ospan0, 2, sample_rate, 0);
  CHECK(static_cast<bool>(opt_offset));
  CHECK(*opt_offset == bufsize / 2);
  CHECK(output0 == expected_output(0, bufsize,
                                   [&data](std::size_t i)
                                   {
                                     return fp_sample(data[i / 2]);
                                   }));

  std::vector<float> output1(bufsize, 0.0F);
  std::span<float> ospan1{&output1[0], output1.size()};
  opt_offset = sound.write_to(ospan1, 2, sample_rate, bufsize / 2);
  CHECK(!opt_offset);
  CHECK(output1 == expected_output(bufsize, 2 * bufsize,
                                   [&data](std::size_t i)
                                   {
                                     if (i >= 32)
                                     {
                                       return 0.0F;
                                     }
                                     return fp_sample(data[i / 2]);
                                   }));
}

TEST_CASE("writes_stereo_data_to_mono_output_in_two_batches", "[sndmix]")
{
  std::unique_ptr<std::int16_t[]> b{new std::int16_t[16]};
  fill(b.get(),
       { 0, -1, 4, -3, 8, -5, 12, -7, 16, -9, 20, -11, 24, -13, 28, -15});
  const auto* data = b.get();
  const auto sample_rate{44100U};
  SoundBuffer sb{std::move(b), 8, 2, sample_rate};
  Sound sound{sb, 1.0};
  const auto bufsize = 5;
  std::vector<float> output0(bufsize, 0.0F);
  std::span<float> ospan0{&output0[0], output0.size()};
  auto opt_offset = sound.write_to(ospan0, 1, sample_rate, 0);
  CHECK(static_cast<bool>(opt_offset));
  CHECK(*opt_offset == bufsize * 2);
  CHECK(output0 == expected_output(0, bufsize,
                                   [&data](std::size_t i)
                                   {
                                     return (fp_sample(data[i * 2])
                                             + fp_sample(data[i * 2 + 1]))
                                       / 2.0F;
                                   }));

  std::vector<float> output1(bufsize, 0.0F);
  std::span<float> ospan1{&output1[0], output1.size()};
  opt_offset = sound.write_to(ospan1, 1, sample_rate, bufsize * 2);
  CHECK(!opt_offset);
  CHECK(output1 == expected_output(bufsize, 2 * bufsize,
                                   [&data](std::size_t i)
                                   {
                                     if (i >= 8)
                                     {
                                       return 0.0F;
                                     }
                                     return (fp_sample(data[i * 2])
                                             + fp_sample(data[i * 2 + 1]))
                                       / 2.0F;
                                   }));
}

TEST_CASE("writes_22050_Hz_mono_data_to_44100_stereo_output", "[sndmix]")
{
  std::unique_ptr<std::int16_t[]> b{new std::int16_t[16]};
  fill(b.get(), { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
  const auto sample_rate{44100U};
  SoundBuffer sb{std::move(b), 16, 1, sample_rate / 2U};
  Sound sound{sb, 1.0};
  std::vector<float> output(64, 0.0F);
  std::span<float> ospan{&output[0], output.size()};
  auto opt_offset = sound.write_to(ospan, 2, sample_rate, 0);
  CHECK(!opt_offset);
  CHECK(output == expected_output(0, output.size(),
                                  [](std::size_t i)
                                  {
                                    if (i >= 62)
                                    {
                                      return 0.0F;
                                    }
                                    return fp_sample(static_cast<std::int16_t>(i) / 2) / 2;
                                  }));
}

TEST_CASE("writes_22050_Hz_stereo_data_to_44100_Hz_mono_output_in_two_batches", "[sndmix]")
{
  const auto data_size{16};
  auto b = std::make_unique<std::int16_t[]>(data_size);
  fill(b.get(),
       { 0, -1, 4, -3, 8, -5, 12, -7, 16, -9, 20, -11, 24, -13, 28, -15});
  const auto* data = b.get();
  const auto sample_rate{44100U};
  SoundBuffer sb{std::move(b), data_size / 2, 2, sample_rate / 2U};
  Sound sound{sb, 1.0};
  const auto bufsize = 10;
  std::vector<float> output0(bufsize, 0.0F);
  std::span<float> ospan0{&output0[0], output0.size()};
  auto opt_offset = sound.write_to(ospan0, 1, sample_rate, 0);
  CHECK(output0 == expected_output(0, bufsize,
                                   [&data](std::size_t i)
                                   {
                                     if (i % 2 == 0)
                                     {
                                       return (fp_sample(data[i])
                                               + fp_sample(data[i + 1]))
                                         / 2.0F;
                                     }
                                     return ((fp_sample(data[i - 1])
                                               + fp_sample(data[i]))
                                              / 2.0F
                                             + (fp_sample(data[i + 1])
                                                 + fp_sample(data[i + 2])) / 2.0F)
                                       / 2.0F;
                                   }));
  REQUIRE(static_cast<bool>(opt_offset));
  CHECK(*opt_offset == 2 * bufsize);

  std::vector<float> output1(bufsize, 0.0F);
  std::span<float> ospan1{&output1[0], output1.size()};
  opt_offset = sound.write_to(ospan1, 1, sample_rate, *opt_offset);
  CHECK(!opt_offset);
  CHECK(output1 == expected_output(bufsize, 2 * bufsize,
                                   [&data](std::size_t i)
                                   {
                                     if (i + 1 >= data_size)
                                     {
                                       return 0.0F;
                                     }
                                     if (i % 2 == 0)
                                     {
                                       return (fp_sample(data[i])
                                               + fp_sample(data[i + 1]))
                                         / 2.0F;
                                     }
                                     if (i + 2 >= data_size)
                                     {
                                       return 0.0F;
                                     }
                                     return (((fp_sample(data[i - 1])
                                               + fp_sample(data[i]))
                                              / 2.0F)
                                             + ((fp_sample(data[i + 1])
                                                 + fp_sample(data[i + 2]))) / 2.0F)
                                       / 2.0F;
                                   }));
}

}  // mt::snd::unittest
