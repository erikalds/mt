#include "project.h"

#include "instrument.h"
#include "note.h"
#include "sample.h"
#include "base64/decode.h"
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>
#include <cassert>
#include <regex>


Project::Project() :
  title{"untitled"}
{
}

Project::~Project() = default;

namespace
{

  Instrument::NoteDef parse_notedef(const std::string& nd)
  {
    const std::string rxs{"([A-G]#?)\\-([0-7])"};
    const std::regex rx{rxs};
    std::smatch mo;
    spdlog::debug("parse notedef {} using regex {}", nd, rxs);

    assert(std::regex_match(nd, mo, rx));
    auto notestr = std::string{mo[1]};
    int n = (static_cast<int>(notestr[0]) - static_cast<int>('C')) * 2;
    constexpr const int notecount{12};
    if (n < 0)
    {
      n = notecount - n + 1;
    }
    else if (n >= 6) // no semi-note between E and F
    {
      n -= 1;
    }
    if (notestr.size() > 1)
    {
      assert(notestr[1] == '#');
      ++n;
    }
    auto octave = static_cast<std::size_t>(std::string{mo[2]}[0] - '0');
    spdlog::debug("Parsed note def: {}-{} [{}-{}]", to_string(Note{n}), octave, n, std::string{mo[2]});
    return Instrument::NoteDef(octave, Note{n});
  }

} // anonymous namespace

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
                                           &pcm_data[0], pcm_data.size(),
                                           sample_node["pitch-offset"].as<float>()});
    }
    std::vector<std::pair<Instrument::NoteDef, Instrument::NoteDef>> assignments{};
    for (const auto& ass_node : node["sample-assignments"])
    {
      Instrument::NoteDef begin = std::make_pair(0, Note::C);
      Instrument::NoteDef end = std::make_pair(7, Note::B);
      if (ass_node["begin"].IsDefined())
      {
        begin = parse_notedef(ass_node["begin"].as<std::string>());
      }
      if (ass_node["end"].IsDefined())
      {
        end = parse_notedef(ass_node["end"].as<std::string>());
      }
      assignments.emplace_back(std::make_pair(begin, end));
    }
    instruments.back().set_sample_assignments(std::move(assignments));
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
