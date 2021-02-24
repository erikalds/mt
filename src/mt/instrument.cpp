#include "instrument.h"

#include "note.h"
#include <SFML/Audio/Sound.hpp>
#include <spdlog/spdlog.h>
#include <cassert>
#include <thread>

Instrument::Instrument(const std::string& filename) :
  sound_buffer{}
{
  if (!sound_buffer.loadFromFile(filename))
  {
    spdlog::error("Failed to load file: {}", filename);
  }
  else
  {
    spdlog::debug("Successfully loaded file: {}", filename);
  }

  const auto constexpr octaves = 8U;
  const auto constexpr notes = 12U;
  sounds.resize(octaves * notes);
  for (auto octave = 0U; octave < octaves; ++octave)
  {
    for (auto note = 0U; note < notes; ++note)
    {
      auto s = std::make_unique<sf::Sound>();
      s->setBuffer(sound_buffer);
      s->setPitch(1.0);
      s->setLoop(true);
      if (octave >= 3)
      {
        s->setPitch(static_cast<float>(octave - 2)
                    + (static_cast<float>(note) / static_cast<float>(notes)));
      }
      sounds[octave * notes + note] = std::move(s);
    }
  }
}

Instrument::Instrument(Instrument&&) noexcept = default;
Instrument& Instrument::operator=(Instrument&&) noexcept = default;
Instrument::~Instrument() = default;

void Instrument::play(std::size_t octave, Note note)
{
  const std::size_t constexpr notes = 12;
  const std::size_t idx = octave * notes + static_cast<std::size_t>(note);
  assert(idx < sounds.size());
  if (sounds[idx]->getStatus() != sf::SoundSource::Playing)
  {
    if (sounds[idx]->getBuffer() == nullptr)
    {
      spdlog::debug("Instrument has no sound_buffer");
      sounds[idx]->setBuffer(sound_buffer);
    }
    spdlog::debug("Play instrument {}-{} [{}]", note, octave, idx);
    sounds[idx]->play();
  }
}

void Instrument::stop(std::size_t octave, Note note)
{
  const std::size_t constexpr notes = 12;
  const std::size_t idx = octave * notes + static_cast<std::size_t>(note);
  assert(idx < sounds.size());
  if (sounds[idx]->getStatus() == sf::SoundSource::Playing)
  {
    spdlog::debug("Stop instrument {}-{} [{}]", note, octave, idx);
    sounds[idx]->stop();
  }
}
