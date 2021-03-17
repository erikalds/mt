#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <memory>
#include <string>
#include <vector>

namespace sf { class Sound; }

enum class Note;
class Sample;

class Instrument
{
public:
  explicit Instrument(std::string name_);
  Instrument(const Instrument&) = delete;
  Instrument& operator=(const Instrument&) = delete;
  Instrument(Instrument&&) noexcept;
  Instrument& operator=(Instrument&&) noexcept;
  ~Instrument();

  [[nodiscard]] std::string_view name() const { return instr_name; }
  void add_sample(Sample&& sample);

  void play(std::size_t octave, Note note);
  void stop(std::size_t octave, Note note);
  void stop();

private:
  [[nodiscard]] const Sample* lookup_sample(std::size_t octave, Note note) const;

  std::vector<Sample> samples;
  std::vector<sf::Sound*> sounds;
  std::string instr_name;
};

#endif /* INSTRUMENT_H */
