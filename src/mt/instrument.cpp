#include "instrument.h"

#include "mtlib/sample.h"
#include "mtlib/note.h"
#include <SFML/Audio/Sound.hpp>
#include <spdlog/spdlog.h>
#include <cassert>
#include <thread>

namespace {

  const auto constexpr octaves = 8U;
  const auto constexpr notes = 12U;

} // anonymous namespace

Instrument::Instrument(std::string name) :
  sounds{octaves * notes, nullptr},
  instr_name{std::move(name)}
{
}

Instrument::Instrument(Instrument&&) noexcept = default;
Instrument& Instrument::operator=(Instrument&&) noexcept = default;

Instrument::~Instrument()
{
  for (auto* s : sounds)
  {
    std::unique_ptr<sf::Sound>{s};
  }
}

void Instrument::add_sample(mt::Sample&& sample)
{
  samples.emplace_back(sample);
}

std::size_t Instrument::sample_count() const
{
  return samples.size();
}

const mt::Sample* Instrument::sample(std::size_t idx) const
{
  if (idx < samples.size())
  {
    return &samples[idx];
  }

  return nullptr;
}

mt::Sample* Instrument::sample(std::size_t idx)
{
  if (idx < samples.size())
  {
    return &samples[idx];
  }

  return nullptr;
}

void Instrument::set_sample_assignments(std::vector<std::pair<NoteDef, NoteDef>> ass)
{
  sample_lut = std::move(ass);
  if (sample_lut.empty())
  {
    sample_lut.emplace_back(std::make_pair(NoteDef{0, mt::Note::C},
                                           NoteDef{7, mt::Note::B}));
  }
  spdlog::debug("Samples: {}", samples.size());
  for (auto i = 0U; i < sample_lut.size(); ++i)
  {
    spdlog::debug(" - {}: [{}-{}] - [{}-{}]", i,
                  sample_lut[i].first.second, sample_lut[i].first.first,
                  sample_lut[i].second.second, sample_lut[i].second.first);
  }
}

void Instrument::play(std::size_t octave, mt::Note note)
{
  const std::size_t idx = octave * notes + static_cast<std::size_t>(note);
  assert(idx < sounds.size());
  auto* sound = sounds[idx];
  if (sound == nullptr)
  {
    const auto* sample = lookup_sample(octave, note);
    if (sample == nullptr)
    {
      spdlog::debug("[{}] No sample found for {}-{} [{}]", name(), note, octave, idx);
      return;
    }
    spdlog::debug("[{}] Create sound {}-{} [{}]", name(), note, octave, idx);
    auto s = sample->create_sound();
    s->setLoop(true);
    if (octave >= 3)
    {
      s->setPitch(sample->get_pitch_offset()
                  + static_cast<float>(octave - 2)
                  + (static_cast<float>(note) / static_cast<float>(notes)));
    }
    spdlog::debug("[{}] Pitch {}", name(), s->getPitch());
    sound = s.get();
    sounds[idx] = s.release();
  }

  spdlog::debug("[{}] Got sound {}-{} [{}]", name(), note, octave, idx);

  if (sound->getStatus() != sf::SoundSource::Playing)
  {
    if (sound->getBuffer() == nullptr)
    {
      spdlog::debug("[{}] Instrument has no sound_buffer", name());
      //sound->setBuffer(sound_buffer);
    }
    spdlog::debug("[{}] Play {}-{} [{}]", name(), note, octave, idx);
    sound->play();
  }
}

void Instrument::stop(std::size_t octave, mt::Note note)
{
  const std::size_t idx = octave * notes + static_cast<std::size_t>(note);
  assert(idx < sounds.size());
  std::unique_ptr<sf::Sound> sound{sounds[idx]};
  if (!sound)
  {
    return;
  }
  sounds[idx] = nullptr;

  if (sound->getStatus() == sf::SoundSource::Playing)
  {
    spdlog::debug("Stop instrument {}-{} [{}]", note, octave, idx);
    sound->stop();
  }
}

void Instrument::stop()
{
  for (auto*& s : sounds)
  {
    std::unique_ptr<sf::Sound> sound{s};
    s = nullptr;
  }
}

namespace
{

  bool operator<=(const Instrument::NoteDef& x, const Instrument::NoteDef& y)
  {
    return x.first < y.first || (x.first == y.first && x.second <= y.second);
  }

} // anonymous namespace

const mt::Sample* Instrument::lookup_sample(std::size_t octave,
                                            mt::Note note) const
{
  std::size_t i{0};
  const NoteDef needle{octave, note};
  for (; i < sample_lut.size(); ++i)
  {
    if (sample_lut[i].first <= needle
        && needle <= sample_lut[i].second)
    {
      return (i < samples.size()) ? &samples[i] : nullptr;
    }
  }

  return nullptr;
}
