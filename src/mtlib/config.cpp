#include "config.h"

#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <fstream>

mt::Config::Config() = default;

[[nodiscard]] std::pair<std::size_t, std::size_t> mt::Config::get_window_size() const
{
  constexpr const auto def_size = std::make_pair(800, 600);
  auto rn = root_node();
  auto szn = rn["MuzakTracker"]["mainwindow"]["size"];
  return std::make_pair(szn["x"].as<int>(def_size.first),
                        szn["y"].as<int>(def_size.second));
}

void mt::Config::set_window_size(const std::pair<std::size_t, std::size_t>& size)
{
  auto rn = root_node();
  auto szn = rn["MuzakTracker"]["mainwindow"]["size"];
  szn["x"] = size.first;
  szn["y"] = size.second;
  save(rn);
}

[[nodiscard]] std::filesystem::path mt::Config::filename() const // NOLINT -- could be made static,
{                                                                // might need state in the future
  return "mt.yml";
}

[[nodiscard]] YAML::Node mt::Config::root_node() const
{
  if (std::filesystem::exists(filename()))
  {
    return YAML::LoadFile(filename());
  }

  return YAML::Node{};
}

void mt::Config::save(const YAML::Node& root_node)
{
  std::ofstream fout{filename()};
  fout << root_node;
}
