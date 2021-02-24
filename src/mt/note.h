#ifndef NOTE_H
#define NOTE_H

#include <fmt/format.h>
#include <iosfwd>

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

std::ostream& operator<<(std::ostream& out, const Note& n);

std::string_view to_string(const Note& n);

template<>
struct fmt::formatter<Note>
{
  constexpr auto parse(format_parse_context& ctx)
  {
    auto it = ctx.begin(), end = ctx.end();
    if (it == end || *it != '}')
      throw format_error{"invalid format"};

    return it;
  }

  template<typename FormatContext>
  auto format(const Note& n, FormatContext& ctx)
  {
    return format_to(ctx.out(), "{}", to_string(n));
  }
};

#endif /* NOTE_H */
