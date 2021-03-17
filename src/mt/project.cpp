#include "project.h"

#include "instrument.h"
#include "sample.h"
#include "base64/decode.h"
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

  for (const auto& node : rootnode["project"]["instruments"])
  {
    spdlog::debug("Loading instrument: {}", node["name"].as<std::string>());
    instruments.emplace_back(Instrument{node["name"].as<std::string>()});
    for (const auto& sample_node : node["samples"])
    {
      auto b64data = sample_node["pcm-data"].as<std::string>();
      std::vector<unsigned char> pcm_data;
      pcm_data.resize(b64::data_size(b64data.c_str()), 0);
      b64::decode(b64data.c_str(),
                  &pcm_data[0], pcm_data.size());
      instruments.back().add_sample(Sample{sample_node["name"].as<std::string>(),
                                           &pcm_data[0], pcm_data.size()});
    }
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
