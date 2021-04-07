#include "project.h"

#include "mtlib/instrument.h"
#include "mtlib/note.h"
#include "mtlib/pattern.h"
#include "mtlib/sample.h"
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>
#include <cassert>
#include <fstream>
#include <regex>


namespace mt {

  Project::Project() :
    title{"untitled"}
  {
    patterns.emplace_back(std::make_unique<Pattern>(0x40, 8)); // for testing
  }

  Project::~Project() = default;

  void Project::load_from_file(const std::filesystem::path& filename)
  {
    auto rootnode = YAML::LoadFile(filename);
    project_filename = filename;
    if (rootnode["project"]["title"].IsDefined())
    {
      title = rootnode["project"]["title"].as<std::string>();
    }

    for (const auto& node : rootnode["project"]["instruments"])
    {
      instruments.emplace_back(Instrument::load_from_yaml(node));
    }
  }

  void Project::save() const
  {
    auto rootnode = YAML::Node{};
    rootnode["project"]["title"] = title;
    for (const auto& instr : instruments)
    {
      rootnode["project"]["instruments"].push_back(instr->get_as_yaml());
    }

    std::ofstream fout(project_filename);
    fout << rootnode;
  }

  void Project::save_as(const std::filesystem::path& filename)
  {
    project_filename = filename;
    save();
  }

  void Project::add_instrument(Instrument instr)
  {
    instruments.emplace_back(std::make_unique<Instrument>(std::move(instr)));
  }

  void Project::remove_instrument(std::size_t idx)
  {
    instruments.erase(instruments.begin() + static_cast<int>(idx));
    // reindex?
  }

  Instrument* Project::get_instrument(std::size_t idx)
  {
    if (idx < instruments.size())
    {
      return instruments[idx].get();
    }

    return nullptr;
  }

  const Instrument* Project::get_instrument(std::size_t idx) const
  {
    if (idx < instruments.size())
    {
      return instruments[idx].get();
    }

    return nullptr;
  }

  std::size_t Project::instrument_count() const
  {
    return instruments.size();
  }

  [[nodiscard]] const Pattern& Project::get_pattern(std::size_t idx) const
  {
    return *patterns[idx];
  }

  [[nodiscard]] Pattern& Project::get_pattern(std::size_t idx)
  {
    return *patterns[idx];
  }

}  // namespace mt
