#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <memory>
#include <string>
#include <vector>

namespace sf { class Sound; }
namespace YAML { class Node; }

namespace mt {

  struct NoteDef;
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
    void remove_sample(std::size_t idx);
    [[nodiscard]] std::size_t sample_count() const;
    [[nodiscard]] const Sample* sample(std::size_t idx) const;
    [[nodiscard]] Sample* sample(std::size_t idx);

    [[nodiscard]] YAML::Node get_as_yaml() const;
    [[nodiscard]] static std::unique_ptr<Instrument> load_from_yaml(const YAML::Node& node);

    void set_sample_assignments(std::vector<std::pair<NoteDef, NoteDef>> ass);

    void play(const NoteDef& notedef);
    void stop(const NoteDef& notedef);
    void stop();

  private:
    [[nodiscard]] const Sample* lookup_sample(const NoteDef& notedef) const;

    std::vector<Sample> samples;
    std::vector<sf::Sound*> sounds;
    std::string instr_name;
    std::vector<std::pair<NoteDef, NoteDef>> sample_lut;
  };

} // namespace mt

#endif /* INSTRUMENT_H */
