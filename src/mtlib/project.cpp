#include "project.h"

#include "mtlib/instrument.h"
#include "mtlib/note.h"
#include "mtlib/sample.h"
#include "base64/decode.h"
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>
#include <cassert>
#include <regex>


namespace mt {

  Project::Project() :
    title{"untitled"}
  {
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
      spdlog::debug("Loading instrument: {}", node["name"].as<std::string>());
      instruments.emplace_back(std::make_unique<Instrument>(node["name"].as<std::string>()));
      for (const auto& sample_node : node["samples"])
      {
        auto b64data = sample_node["pcm-data"].as<std::string>();
        std::vector<unsigned char> pcm_data;
        pcm_data.resize(b64::data_size(b64data.c_str()), 0);
        b64::decode(b64data.c_str(),
                    &pcm_data[0], pcm_data.size());
        instruments.back()->add_sample(Sample{sample_node["name"].as<std::string>(),
                                              &pcm_data[0], pcm_data.size(),
                                              sample_node["pitch-offset"].as<float>()});
      }
      std::vector<std::pair<NoteDef, NoteDef>> assignments{};
      for (const auto& ass_node : node["sample-assignments"])
      {
        NoteDef begin{Note::C, 0};
        NoteDef end{Note::B, 7};
        if (ass_node["begin"].IsDefined())
        {
          begin = NoteDef::from_string(ass_node["begin"].as<std::string>());
        }
        if (ass_node["end"].IsDefined())
        {
          end = NoteDef::from_string(ass_node["end"].as<std::string>());
        }
        assignments.emplace_back(std::make_pair(begin, end));
      }
      instruments.back()->set_sample_assignments(std::move(assignments));
    }
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

}  // namespace mt