#include "instrument.h"

#include "note.h"
#include <SFML/Audio/Sound.hpp>
#include <spdlog/spdlog.h>
#include <cassert>
#include <thread>

namespace {

  const auto constexpr octaves = 8U;
  const auto constexpr notes = 12U;

} // anonymous namespace

Instrument::Instrument(const std::string& filename) :
  sound_buffer{},
  sounds{octaves * notes, nullptr},
  instr_name{filename}
{
  if (!sound_buffer.loadFromFile(filename))
  {
    spdlog::error("Failed to load file: {}", filename);
  }
  else
  {
    spdlog::debug("Successfully loaded file: {}", filename);
  }
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

void Instrument::play(std::size_t octave, Note note)
{
  const std::size_t idx = octave * notes + static_cast<std::size_t>(note);
  assert(idx < sounds.size());
  auto* sound = sounds[idx];
  if (sound == nullptr)
  {
    spdlog::debug("[{}] Create sound {}-{} [{}]", name(), note, octave, idx);
    auto s = std::make_unique<sf::Sound>();
    s->setBuffer(sound_buffer);
    s->setPitch(1.0);
    s->setLoop(true);
    if (octave >= 3)
    {
      s->setPitch(static_cast<float>(octave - 2)
                  + (static_cast<float>(note) / static_cast<float>(notes)));
    }
    sound = s.get();
    sounds[idx] = s.release();
  }

  spdlog::debug("[{}] Got sound {}-{} [{}]", name(), note, octave, idx);

  if (sound->getStatus() != sf::SoundSource::Playing)
  {
    if (sound->getBuffer() == nullptr)
    {
      spdlog::debug("[{}] Instrument has no sound_buffer", name());
      sound->setBuffer(sound_buffer);
    }
    spdlog::debug("[{}] Play {}-{} [{}]", name(), note, octave, idx);
    sound->play();
  }
}

void Instrument::stop(std::size_t octave, Note note)
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
