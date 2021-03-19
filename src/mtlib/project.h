#ifndef PROJECT_H
#define PROJECT_H

#include <filesystem>
#include <string>
#include <vector>

namespace mt {

  class Instrument;

  /// Document class for one musical project/song.
  class Project
  {
  public:
    Project();
    ~Project();

    void load_from_file(const std::filesystem::path& filename);

    [[nodiscard]] Instrument* get_instrument(std::size_t idx);
    [[nodiscard]] const Instrument* get_instrument(std::size_t idx) const;
    [[nodiscard]] std::size_t instrument_count() const;
    [[nodiscard]] std::string_view get_title() const { return title; }

  private:
    std::string title;
    std::string project_filename;
    std::vector<Instrument> instruments;
  };

} // namespace mt

#endif /* PROJECT_H */
