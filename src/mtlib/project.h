#ifndef PROJECT_H
#define PROJECT_H

#include <filesystem>
#include <memory>
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
    void save() const;
    void save_as(const std::filesystem::path& filename);

    void add_instrument(Instrument instr);
    void remove_instrument(std::size_t idx);

    [[nodiscard]] Instrument* get_instrument(std::size_t idx);
    [[nodiscard]] const Instrument* get_instrument(std::size_t idx) const;
    [[nodiscard]] std::size_t instrument_count() const;

    [[nodiscard]] std::string_view get_title() const { return title; }
    [[nodiscard]] std::filesystem::path get_filename() const { return project_filename; }

  private:
    std::string title;
    std::filesystem::path project_filename;
    std::vector<std::unique_ptr<Instrument>> instruments;
  };

} // namespace mt

#endif /* PROJECT_H */
