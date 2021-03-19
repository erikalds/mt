#include "instrument.h"

#include "mtlib/sample.h"
#include "mtlib/note.h"
#include <SFML/Audio/Sound.hpp>
#include <spdlog/spdlog.h>
#include <cassert>
#include <thread>

namespace mt {
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

  void Instrument::add_sample(Sample&& sample)
  {
    samples.emplace_back(sample);
  }

  std::size_t Instrument::sample_count() const
  {
    return samples.size();
  }

  const Sample* Instrument::sample(std::size_t idx) const
  {
    if (idx < samples.size())
    {
      return &samples[idx];
    }

    return nullptr;
  }

  Sample* Instrument::sample(std::size_t idx)
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
      sample_lut.emplace_back(std::make_pair(NoteDef{Note::C, 0},
                                             NoteDef{Note::B, 7}));
    }
    spdlog::debug("Samples: {}", samples.size());
    for (auto i = 0U; i < sample_lut.size(); ++i)
    {
      spdlog::debug(" - {}: [{}] - [{}]", i,
                    sample_lut[i].first, sample_lut[i].second);
    }
  }

  void Instrument::play(const NoteDef& notedef)
  {
    const std::size_t idx = notedef.octave * notes + static_cast<std::size_t>(notedef.note);
    assert(idx < sounds.size());
    auto* sound = sounds[idx];
    if (sound == nullptr)
    {
      const auto* sample = lookup_sample(notedef);
      if (sample == nullptr)
      {
        spdlog::debug("[{}] No sample found for {} [{}]", name(), notedef, idx);
        return;
      }
      spdlog::debug("[{}] Create sound {} [{}]", name(), notedef, idx);
      auto s = sample->create_sound();
      s->setLoop(true);
      if (notedef.octave >= 3)
      {
        s->setPitch(sample->get_pitch_offset()
                    + static_cast<float>(notedef.octave - 2)
                    + (static_cast<float>(notedef.note) / static_cast<float>(notes)));
      }
      spdlog::debug("[{}] Pitch {}", name(), s->getPitch());
      sound = s.get();
      sounds[idx] = s.release();
    }

    spdlog::debug("[{}] Got sound {} [{}]", name(), notedef, idx);

    if (sound->getStatus() != sf::SoundSource::Playing)
    {
      if (sound->getBuffer() == nullptr)
      {
        spdlog::debug("[{}] Instrument has no sound_buffer", name());
        //sound->setBuffer(sound_buffer);
      }
      spdlog::debug("[{}] Play {} [{}]", name(), notedef, idx);
      sound->play();
    }
  }

  void Instrument::stop(const NoteDef& notedef)
  {
    const std::size_t idx = notedef.octave * notes + static_cast<std::size_t>(notedef.note);
    assert(idx < sounds.size());
    std::unique_ptr<sf::Sound> sound{sounds[idx]};
    if (!sound)
    {
      return;
    }
    sounds[idx] = nullptr;

    if (sound->getStatus() == sf::SoundSource::Playing)
    {
      spdlog::debug("Stop instrument {} [{}]", notedef, idx);
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

  const Sample* Instrument::lookup_sample(const NoteDef& notedef) const
  {
    std::size_t i{0};
    for (; i < sample_lut.size(); ++i)
    {
      if (sample_lut[i].first <= notedef
          && notedef <= sample_lut[i].second)
      {
        return (i < samples.size()) ? &samples[i] : nullptr;
      }
    }

    return nullptr;
  }

}  // namespace mt
