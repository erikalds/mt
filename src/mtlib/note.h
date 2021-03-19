#ifndef NOTE_H
#define NOTE_H

#include <compare>
#include <fmt/format.h>
#include <iosfwd>

namespace mt {

  enum class Note
  {
   C = 0,
   C_sharp = 1,
   D = 2,
   D_sharp = 3,
   E = 4,
   F = 5,
   F_sharp = 6,
   G = 7,
   G_sharp = 8,
   A = 9,
   A_sharp = 10,
   B = 11
  };

  struct NoteDef
  {
    NoteDef(Note n, std::size_t oct) : note{n}, octave{oct} {}

    static NoteDef from_string(std::string_view s);

    Note note;
    std::size_t octave;

    auto operator<=>(const NoteDef& x) const
    { return (this->note == x.note && this->octave == x.octave) ?
        std::strong_ordering::equal :
        (this->octave < x.octave
         || (this->octave == x.octave && this->note < x.note)) ?
        std::strong_ordering::less :
        std::strong_ordering::greater; }

    bool operator==(const NoteDef&) const = default;
  };

  std::ostream& operator<<(std::ostream& out, const Note& n);
  std::ostream& operator<<(std::ostream& out, const NoteDef& n);

  std::string_view to_string(const Note& n);
  std::string to_string(const NoteDef& nd);

}  // namespace mt

template<>
struct fmt::formatter<mt::Note>
{
  constexpr auto parse(format_parse_context& ctx)
  {
    const auto* it = ctx.begin();
    const auto* end = ctx.end();
    if (it == end || *it != '}')
    {
      throw format_error{"invalid format"};
    }

    return it;
  }

  template<typename FormatContext>
  auto format(const mt::Note& n, FormatContext& ctx)
  {
    return format_to(ctx.out(), "{}", mt::to_string(n));
  }
};

template<>
struct fmt::formatter<mt::NoteDef>
{
  constexpr auto parse(format_parse_context& ctx)
  {
    const auto* it = ctx.begin();
    const auto* end = ctx.end();
    if (it == end || *it != '}')
    {
      throw format_error{"invalid format"};
    }

    return it;
  }

  template<typename FormatContext>
  auto format(const mt::NoteDef& nd, FormatContext& ctx)
  {
    return format_to(ctx.out(), "{}", mt::to_string(nd));
  }
};


#endif /* NOTE_H */
