#include "project.h"

#include "base64/decode.h"
#include "instrument.h"
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>


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

  if (rootnode["project"]["instruments"].IsDefined())
  {
    spdlog::debug("instruments is defined in project rootnode");
    if (rootnode["project"]["instruments"].IsSequence())
    {
      for (const auto& node : rootnode["project"]["instruments"])
      {
        spdlog::debug("Loading instrument: {}", node["name"].as<std::string>());
        spdlog::debug("First pcm-data: {}", node["pcm-data"].as<std::string>().substr(0, 100));
        instruments.emplace_back(Instrument{node["name"].as<std::string>()});
        auto b64data = node["pcm-data"].as<std::string>();
        std::vector<unsigned char> pcm_data;
        pcm_data.resize(b64::data_size(b64data.c_str()), 0);
        b64::decode(b64data.c_str(),
                    &pcm_data[0], pcm_data.size());
        instruments.back().load_pcm_data(&pcm_data[0], pcm_data.size());
      }
    }
  }
  else
  {
    spdlog::debug("projects->instruments is not defined");
  }
}

Instrument* Project::get_instrument(std::size_t idx)
{
  if (idx < instruments.size())
  {
    return &instruments[idx];
  }

  return nullptr;
}

const Instrument* Project::get_instrument(std::size_t idx) const
{
  if (idx < instruments.size())
  {
    return &instruments[idx];
  }

  return nullptr;
}

std::size_t Project::instrument_count() const
{
  return instruments.size();
}
