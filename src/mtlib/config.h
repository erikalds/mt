#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>
#include <utility>

namespace YAML { class Node; }

namespace mt {

  // Config data for MuzakTracker
  class Config
  {
  public:
    Config();

    [[nodiscard]] std::pair<std::size_t, std::size_t> get_window_size() const;
    void set_window_size(const std::pair<std::size_t, std::size_t>& size);

  private:
    [[nodiscard]] std::filesystem::path filename() const;

    [[nodiscard]] YAML::Node root_node() const;
    void save(const YAML::Node& root_node);
  };

}  // namespace mt

#endif /* CONFIG_H */
