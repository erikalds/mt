#include "soundbuffer.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>

namespace mt::snd {

  SoundBuffer::SoundBuffer(const SoundBuffer& other) :
    samples{new std::int16_t[other.sample_count * other.channel_count]},
    fsamples{new float[other.sample_count * other.channel_count]},
    sample_count{other.sample_count},
    sample_rate{other.sample_rate},
    channel_count{other.channel_count}
  {
    const auto n = sample_count * channel_count;
    std::copy(other.samples.get(), other.samples.get() + n, samples.get());
    std::copy(other.fsamples.get(), other.fsamples.get() + n, fsamples.get());
  }

  SoundBuffer& SoundBuffer::operator=(const SoundBuffer& other)
  {
    if (this != &other)
    {
      sample_count = other.sample_count;
      sample_rate = other.sample_rate;
      channel_count = other.channel_count;
      const auto n = sample_count * channel_count;
      samples.reset(new std::int16_t[other.sample_count * other.channel_count]);
      fsamples.reset(new float[other.sample_count * other.channel_count]);
      std::copy(other.samples.get(), other.samples.get() + n, samples.get());
      std::copy(other.fsamples.get(), other.fsamples.get() + n, fsamples.get());
    }
    return *this;
  }

  SoundBuffer::SoundBuffer(std::unique_ptr<std::int16_t[]> samples_,
                           std::uint32_t sample_count_,
                           std::uint16_t channel_count_,
                           std::uint32_t sample_rate_) :
    samples{std::move(samples_)},
    fsamples{new float[sample_count_ * channel_count_]},
    sample_count{sample_count_},
    sample_rate{sample_rate_},
    channel_count{channel_count_}
  {
    std::transform(samples.get(),
                   samples.get() + (sample_count * channel_count),
                   fsamples.get(),
                   [](std::int16_t i)
                   {
                     const float maxval = 32768;
                     return static_cast<float>(i) / maxval;
                   });
  }

  [[nodiscard]] const std::int16_t* SoundBuffer::get_samples() const
  {
    return samples.get();
  }

  [[nodiscard]] const float* SoundBuffer::get_float_samples() const
  {
    return fsamples.get();
  }

  [[nodiscard]] std::uint32_t SoundBuffer::get_sample_count() const
  {
    return sample_count;
  }

  [[nodiscard]] std::uint32_t SoundBuffer::get_sample_rate() const
  {
    return sample_rate;
  }

  [[nodiscard]] std::chrono::microseconds SoundBuffer::get_duration() const
  {
    auto dur_s = static_cast<double>(get_sample_count()) / get_sample_rate();
    const auto usps = 1.0e6;
    auto dur_us = std::round(dur_s * usps);
    return std::chrono::microseconds(static_cast<std::uint64_t>(dur_us));
  }

  [[nodiscard]] std::uint16_t SoundBuffer::get_channel_count() const
  {
    return channel_count;
  }

  // class Sound{};

  // [[nodiscard]] std::unique_ptr<Sound> SoundBuffer::create_sound() const
  // {
  //   return {};
  // }

}  // namespace mt::snd
