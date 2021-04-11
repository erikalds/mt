#ifndef PATTERN_H
#define PATTERN_H

#include "mtlib/note.h"
#include <bits/c++config.h>
#include <cstdint>
#include <deque>
#include <optional>
#include <string>
#include <vector>

namespace mt {

  struct NoteEvent
  {
    explicit NoteEvent(std::size_t mod_size) :
      mod(mod_size, std::optional<std::uint16_t>{}) {}

    [[nodiscard]] std::string represent() const;

    /// Note and octave to play when this NoteEvent is encountered.
    std::optional<NoteDef> note; // NOLINT - public visibility
    /// Instrument to play.
    std::optional<std::uint8_t> instr; // NOLINT - public visibility
    /// Volume to play the instrument at.
    std::optional<std::uint8_t> volume; // NOLINT - public visibility
    /// Modifiers/effects to apply to the sample.
    std::vector<std::optional<std::uint16_t>> mod; // NOLINT - public visibility
    /// This event is a signal to the currently playing instrument to stop.
    bool stop = false; // NOLINT - public visibility
  };

  class Track
  {
  public:
    explicit Track(std::size_t length) : track{length, NoteEvent{1U}} {}

    [[nodiscard]] const NoteEvent& operator[](std::size_t idx) const
    { return track[idx]; }
    [[nodiscard]] NoteEvent& operator[](std::size_t idx)
    { return track[idx]; }
    [[nodiscard]] std::size_t size() const { return track.size(); }

    using iterator = std::vector<NoteEvent>::iterator;
    using const_iterator = std::vector<NoteEvent>::const_iterator;
    [[nodiscard]] iterator begin() { return track.begin(); }
    [[nodiscard]] iterator end() { return track.end(); }
    [[nodiscard]] const_iterator begin() const { return track.begin(); }
    [[nodiscard]] const_iterator end() const { return track.end(); }

  private:
    std::vector<NoteEvent> track;
  };


  class Pattern
  {
  public:
    Pattern(std::string pattern_name, std::size_t length,
            std::size_t track_count) :
      pattern{track_count, Track{length}}, name_{std::move(pattern_name)} {}

    [[nodiscard]] std::string_view name() const { return name_; }
    void set_name(std::string_view new_name) { name_ = new_name; }

    [[nodiscard]] const Track& operator[](std::size_t idx) const
    { return pattern[idx]; }
    [[nodiscard]] Track& operator[](std::size_t idx)
    { return pattern[idx]; }
    [[nodiscard]] std::size_t size() const
    { return pattern.size(); }

    using iterator = std::vector<Track>::iterator;
    using const_iterator = std::vector<Track>::const_iterator;
    [[nodiscard]] iterator begin() { return pattern.begin(); }
    [[nodiscard]] iterator end() { return pattern.end(); }
    [[nodiscard]] const_iterator begin() const { return pattern.begin(); }
    [[nodiscard]] const_iterator end() const { return pattern.end(); }

    [[nodiscard]] std::size_t get_track_length() const
    { return pattern[0].size(); }

  private:
    std::vector<Track> pattern;
    std::string name_;
  };

}  // namespace mt

#endif /* PATTERN_H */
