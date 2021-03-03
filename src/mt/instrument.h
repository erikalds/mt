#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <SFML/Audio/SoundBuffer.hpp>
#include <memory>
#include <string>

namespace sf { class Sound; }

enum class Note;

class Instrument
{
public:
  explicit Instrument(const std::string& filename);
  Instrument(const Instrument&) = delete;
  Instrument& operator=(const Instrument&) = delete;
  Instrument(Instrument&&) noexcept;
  Instrument& operator=(Instrument&&) noexcept;
  ~Instrument();

  void play(std::size_t octave, Note note);
  void stop(std::size_t octave, Note note);

private:
  sf::SoundBuffer sound_buffer;
  std::vector<sf::Sound*> sounds;
};

#endif /* INSTRUMENT_H */
