#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <memory>
#include <string>
#include <vector>

namespace sf { class Sound; }

namespace mt { enum class Note; }
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
  void set_name(std::string_view n) { instr_name = n; }

  void add_sample(Sample&& sample);
  [[nodiscard]] std::size_t sample_count() const;
  [[nodiscard]] const Sample* sample(std::size_t idx) const;
  [[nodiscard]] Sample* sample(std::size_t idx);

  using NoteDef = std::pair<std::size_t, mt::Note>;
  void set_sample_assignments(std::vector<std::pair<NoteDef, NoteDef>> ass);

  void play(std::size_t octave, mt::Note note);
  void stop(std::size_t octave, mt::Note note);
  void stop();

private:
  [[nodiscard]] const Sample* lookup_sample(std::size_t octave, mt::Note note) const;

  std::vector<Sample> samples;
  std::vector<sf::Sound*> sounds;
  std::string instr_name;
  std::vector<std::pair<NoteDef, NoteDef>> sample_lut;
};

#endif /* INSTRUMENT_H */
