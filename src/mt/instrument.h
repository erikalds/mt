#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <SFML/Audio/SoundBuffer.hpp>
#include <memory>
#include <string>
#include <list>

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

  std::string_view name() const { return instr_name; }

  void play(std::size_t octave, Note note);
  void stop(std::size_t octave, Note note);
  void stop();

private:
  sf::SoundBuffer sound_buffer;
  std::vector<sf::Sound*> sounds;
  std::string instr_name;
};

#endif /* INSTRUMENT_H */
