#include "pattern.h"

#include <memory>
#include <yaml-cpp/yaml.h>

#include <cstdint>
#include <iomanip>
#include <sstream>

namespace mt {

  namespace {

    std::string to_str(const std::optional<NoteDef>& nd, bool stop)
    {
      if (stop)
      {
        return "===";
      }
      if (!nd)
      {
        return "---";
      }
      return to_string(*nd);
    }

    template<typename T>
    std::string to_str(const std::optional<T>& v)
    {
      constexpr const auto w{sizeof(T) * 2};
      if (!v)
      {
        return std::string(w, '-');
      }

      std::ostringstream ost;
      ost << std::setw(w) << std::setfill('0') << std::hex << std::fixed
          << static_cast<int>(*v);
      return ost.str();
    }

  } // anonymous namespace

  [[nodiscard]] std::string NoteEvent::represent() const
  {
    std::string mods{};
    for (const auto& m : mod)
    {
      mods += " " + to_str(m);
    }
    return to_str(note, stop) + " " + to_str(instr) + " " + to_str(volume) + mods;
  }

  namespace {

    template<typename T, typename U>
    void assign(std::optional<U>& dest, const YAML::Node& node,
                std::string_view name)
    {
      if (node[name.data()] != nullptr)
      {
        dest = node[name.data()].as<T>();
      }
    }

    template<>
    void assign<std::string>(std::optional<NoteDef>& dest, const YAML::Node& node,
                             std::string_view name)
    {
      if (node[name.data()] != nullptr)
      {
        dest = NoteDef::from_string(node[name.data()].as<std::string>());
      }
    }

  } // anonymous namespace

  NoteEvent NoteEvent::load_from_yaml(const YAML::Node& node)
  {
    auto modsize = 1UL;
    if (node["mod"] != nullptr)
    {
      modsize = node["mod"].size();
    }

    NoteEvent ne{std::max(1UL, modsize)};
    assign<std::string>(ne.note, node, "note");
    assign<int>(ne.instr, node, "instr");
    if (node["stop"] != nullptr)
    {
      ne.stop = node["stop"].as<bool>();
    }
    assign<int>(ne.volume, node, "volume");
    if (node["mod"] != nullptr)
    {
      for (std::size_t i{0}; i < node["mod"].size(); ++i)
      {
        ne.mod[i] = node["mod"][i].as<int>();
      }
    }
    return ne;
  }

  std::optional<YAML::Node> NoteEvent::get_as_yaml() const
  {
    if (!note && !instr && !volume && !stop)
    {
      bool empty = true;
      for (auto m : mod)
      {
        empty = empty && !bool(m);
      }
      if (empty)
      {
        return {};
      }
    }

    YAML::Node n{};
    if (stop)
    {
      n["stop"] = true;
    }
    if (note)
    {
      n["note"] = mt::to_string(*note);
    }
    if (instr)
    {
      n["instr"] = static_cast<int>(*instr);
    }
    if (volume)
    {
      n["volume"] = static_cast<int>(*volume);
    }

    for (std::size_t i = 0; i < mod.size(); ++i)
    {
      if (mod[i])
      {
        n["mod"][i] = *mod[i];
      }
    }

    return n;
  }

  bool operator==(const NoteEvent& x, const NoteEvent& y)
  {
    return x.note == y.note
      && x.instr == y.instr
      && x.volume == y.volume
      && x.stop == y.stop
      && x.mod == y.mod;
  }

  std::ostream& operator<<(std::ostream& ost, const NoteEvent& x)
  {
    return ost << x.represent();
  }


  Track Track::load_from_yaml(const YAML::Node& node)
  {
    Track track{node["length"].as<std::size_t>()};
    if (node["events"] != nullptr)
    {
      for (std::size_t i = 0; i < track.size(); ++i)
      {
        if (node["events"][i] != nullptr)
        {
          track[i] = NoteEvent::load_from_yaml(node["events"][i]);
        }
      }
    }
    return track;
  }

  YAML::Node Track::get_as_yaml() const
  {
    YAML::Node node{};
    node["length"] = track.size();
    for (auto i = 0U; i < track.size(); ++i)
    {
      auto opt_note_event_node = track[i].get_as_yaml();
      if (opt_note_event_node)
      {
        node["events"][i] = *opt_note_event_node;
      }
    }
    return node;
  }


  std::unique_ptr<Pattern> Pattern::load_from_yaml(const YAML::Node& node)
  {
    auto pattern = std::make_unique<Pattern>(node["name"].as<std::string>(),
                                             0x40,
                                             node["track_count"].as<std::size_t>());
    for (auto i = 0U; i < pattern->size(); ++i)
    {
      (*pattern)[i] = Track::load_from_yaml(node["tracks"][i]);
    }
    return pattern;
  }

  YAML::Node Pattern::get_as_yaml() const
  {
    YAML::Node node{};
    node["name"] = name_;
    node["track_count"] = size();
    for (auto i = 0U; i < size(); ++i)
    {
      node["tracks"][i] = pattern[i].get_as_yaml();
    }
    return node;
  }

}  // namespace mt
