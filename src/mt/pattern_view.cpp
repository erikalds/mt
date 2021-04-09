#include "pattern_view.h"

#include "keyboard.h"
#include "mtlib/pattern.h"
#include <SFML/Window/Keyboard.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <SFML/Window/Event.hpp>
#include <sstream>

PatternView::PatternView() :
  digg::SubWindow("Pattern")
{
}

void PatternView::display_pattern(mt::Pattern& p)
{
  current_pattern = &p;

  const auto N = current_pattern->size() + 1;
  const auto row_title_width = 25;
  column_widths.clear();
  column_widths.reserve(N);
  column_widths.push_back(row_title_width);
  const auto col_width{115};
  column_widths.insert(std::end(column_widths),
                       current_pattern->size(), col_width);

  header_strings.clear();
  headers.clear();
  header_strings.reserve(N);
  headers.reserve(N);
  header_strings.emplace_back(std::string{});
  headers.push_back(header_strings.back().c_str());
  for (auto col = 0U; col < current_pattern->size(); ++col)
  {
    header_strings.emplace_back(fmt::format("Track {}", col));
    headers.push_back(header_strings.back().c_str());
  }
}

void PatternView::selected_instrument_changed(int instr_idx,
                                              mt::Instrument* /*instr*/,
                                              mt::Sample* /*sample*/)
{
  current_instrument = instr_idx;
}

namespace {

  /// Wrap value in range [0, end>.
  template<typename T, typename U>
  T wrap_pos(T value, U end)
  {
    if (value < 0)
    {
      return value + static_cast<T>(end);
    }

    if (value >= static_cast<T>(end))
    {
      return value - static_cast<T>(end);
    }

    return value;
  }

} // anonymous namespace

void PatternView::event_occurred(const sf::Event& e)
{
  if (!is_active()
      || !(e.type == sf::Event::KeyPressed
           || e.type == sf::Event::KeyReleased))
  {
    return;
  }

  if (e.type == sf::Event::KeyPressed)
  {
    if (e.key.code == sf::Keyboard::Down)
    {
      ++current_row;
    }
    else if (e.key.code == sf::Keyboard::Up)
    {
      --current_row;
    }
    else if (e.key.code == sf::Keyboard::Left)
    {
      --current_subcolumn;
    }
    else if (e.key.code == sf::Keyboard::Right)
    {
      ++current_subcolumn;
    }
    else if (e.key.code == sf::Keyboard::Tab)
    {
      if (e.key.shift == true)
      {
        --current_column;
      }
      else
      {
        ++current_column;
      }
    }
    else if (e.key.code == sf::Keyboard::Space)
    {
      editing = !editing;
    }
    else if (editing)
    {
      if (set_current_note_event(e))
      {
        ++current_row;
      }
    }
    else
    {
      return;
    }

    current_row = wrap_pos(current_row, current_pattern->get_track_length());
    current_column = wrap_pos(current_column, current_pattern->size());
    constexpr const auto subcol_count{9};
    current_subcolumn = wrap_pos(current_subcolumn, subcol_count);
    spdlog::debug("[Pattern] current column/row: {}[{}]/{}",
                  current_column, current_subcolumn, current_row);
  }
  else if (e.type == sf::Event::KeyReleased)
  {
    spdlog::debug("[Pattern] key released: {}", e.key.code);
  }
}

namespace {

  std::optional<std::uint32_t> keycode_to_hex(sf::Keyboard::Key keycode)
  {
    switch (keycode)
    {
    case sf::Keyboard::Num0:         ///< The 0 key
    case sf::Keyboard::Numpad0:      ///< The numpad 0 key
      return 0;
    case sf::Keyboard::Num1:         ///< The 1 key
    case sf::Keyboard::Numpad1:      ///< The numpad 1 key
      return 1;
    case sf::Keyboard::Num2:         ///< The 2 key
    case sf::Keyboard::Numpad2:      ///< The numpad 2 key
      return 2;
    case sf::Keyboard::Num3:         ///< The 3 key
    case sf::Keyboard::Numpad3:      ///< The numpad 3 key
      return 3;
    case sf::Keyboard::Num4:         ///< The 4 key
    case sf::Keyboard::Numpad4:      ///< The numpad 4 key
      return 4;
    case sf::Keyboard::Num5:         ///< The 5 key
    case sf::Keyboard::Numpad5:      ///< The numpad 5 key
      return 5;
    case sf::Keyboard::Num6:         ///< The 6 key
    case sf::Keyboard::Numpad6:      ///< The numpad 6 key
      return 6;
    case sf::Keyboard::Num7:         ///< The 7 key
    case sf::Keyboard::Numpad7:      ///< The numpad 7 key
      return 7;
    case sf::Keyboard::Num8:         ///< The 8 key
    case sf::Keyboard::Numpad8:      ///< The numpad 8 key
      return 8;
    case sf::Keyboard::Num9:         ///< The 9 key
    case sf::Keyboard::Numpad9:      ///< The numpad 9 key
      return 9;
    case sf::Keyboard::A:            ///< The A key
      return 0xA;
    case sf::Keyboard::B:            ///< The B key
      return 0xB;
    case sf::Keyboard::C:            ///< The C key
      return 0xC;
    case sf::Keyboard::D:            ///< The D key
      return 0xD;
    case sf::Keyboard::E:            ///< The E key
      return 0xE;
    case sf::Keyboard::F:            ///< The F key
      return 0xF;
    default:
      return std::optional<std::uint32_t>{};
    }
  }

  template<typename T, typename U>
  void update_optional(std::optional<T>& dest, std::size_t offset, U val)
  {
    T origval = 0;
    if (dest)
    {
      origval = *dest;
    }

    constexpr const auto bitsperhalfbyte{4};
    const auto mask{0xFU << (offset * bitsperhalfbyte)};
    dest = (origval & ~mask) | (((val) << (offset * bitsperhalfbyte)) & mask);
  }

  template<typename T>
  bool edit_field(std::optional<T>& field, std::size_t fieldcolumn, const sf::Event& e)
  {
    auto opt_hex_char = keycode_to_hex(e.key.code);
    if (!opt_hex_char)
    {
      if (e.key.code == sf::Keyboard::Delete)
      {
        field = std::optional<T>{};
        return true;
      }
      return false;
    }
    constexpr const auto maxcol{(sizeof(T) * 2) - 1};
    update_optional(field, maxcol - fieldcolumn, *opt_hex_char);
    return true;
  }

} // anonymous namespace

bool PatternView::set_current_note_event(const sf::Event& e)
{
  const auto i = static_cast<std::size_t>(current_column);
  const auto j = static_cast<std::size_t>(current_row);

  if (current_subcolumn == 0)
  {
    if (keyboard == nullptr)
    {
      return false;
    }
    auto opt_notedef = keyboard->get_note_from_event(e);
    if (!opt_notedef)
    {
      if (e.key.code == sf::Keyboard::LControl)
      {
        (*current_pattern)[i][j].stop = true;
        return true;
      }
      if (e.key.code == sf::Keyboard::Delete)
      {
        (*current_pattern)[i][j].stop = false;
        (*current_pattern)[i][j].note = std::optional<mt::NoteDef>{};
        (*current_pattern)[i][j].instr = std::optional<std::uint8_t>{};
        return true;
      }
      return false;
    }
    (*current_pattern)[i][j].stop = false;
    (*current_pattern)[i][j].note = *opt_notedef;
    (*current_pattern)[i][j].instr = current_instrument;
    return true;
  }

  if (current_subcolumn < 3)
  {
    const auto instrcol = (static_cast<std::size_t>(current_subcolumn) - 1U);
    return edit_field((*current_pattern)[i][j].instr, instrcol, e);
  }
  if (current_subcolumn < 5)
  {
    const auto volcol = (static_cast<std::size_t>(current_subcolumn) - 3U);
    return edit_field((*current_pattern)[i][j].volume, volcol, e);
  }

  const auto modcol = static_cast<std::size_t>(current_subcolumn) - 5U;
  const auto modidx = modcol / 4;
  return edit_field((*current_pattern)[i][j].mod[modidx], modcol % 4, e);
}

void PatternView::draw_widgets()
{
  column = 0U;
  render_column_header(current_pattern->size() + 1);
  render_row_header(current_pattern->get_track_length());
  for (const auto& track : *current_pattern)
  {
    std::vector<std::string> trackdata;
    trackdata.reserve(track.size());
    for (const auto& note_event : track)
    {
      trackdata.emplace_back(note_event.represent());
    }
    render_column(trackdata);
  }
}

void PatternView::render_column_header(std::size_t length)
{
  for (auto i = 0; i < static_cast<int>(length); ++i)
  {
    if (i > 0)
    {
      ImGui::SameLine();
    }
    if (i - 1 == current_column)
    {
      ImGui::SetScrollHereX();
    }

    std::ostringstream ost;
    ost << "Pattern-header-" << i;
    const ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar
      | ImGuiWindowFlags_NoScrollWithMouse;
    const ImVec2 childsize{column_widths[static_cast<std::size_t>(i)],
                           ImGui::GetTextLineHeight() * 2.0F};
    if (ImGui::BeginChild(ost.str().c_str(), childsize, true, flags))
    {
      if (i == 0 && editing)
      {
        auto center = ImGui::GetCursorScreenPos();
        center.x += childsize.x / 5;
        center.y += childsize.y / 5;
        float radius = (std::min(childsize.x, childsize.y) / 2) - 4;
        ImGui::GetForegroundDrawList()->AddCircleFilled(center, radius, 0xFF0000FF);
      }
      else
      {
        ImGui::TextColored(ImVec4(1.0F, 1.0F, 1.0F, 1.0F), "%s",
                           headers[static_cast<std::size_t>(i)]);
      }
    }
    ImGui::EndChild();
  }
}

void PatternView::render_row_header(std::size_t length)
{
  std::vector<std::string> data;
  data.resize(length);
  std::size_t i = 0U;
  std::generate(std::begin(data), std::end(data), [&i]() { return fmt::format("{:02x}", i++); });
  render_column(data);
}

void PatternView::render_column(const std::vector<std::string>& data)
{
  if (column > 0)
  {
    ImGui::SameLine();
  }

  const ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar
    | ImGuiWindowFlags_NoScrollWithMouse;
  if (column - 1 == current_column)
  {
    ImGui::SetScrollHereX();
    ImGui::PushStyleColor(ImGuiCol_ChildBg, 0x40444444);
  }
  else
  {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, 0x40222222);
  }

  std::ostringstream ost;
  ost << "Track-" << column;
  const ImVec2 childsize{column_widths[static_cast<std::size_t>(column)], 0};
  if (ImGui::BeginChild(ost.str().c_str(), childsize, true, flags))
  {
    auto row = 0;
    for (const auto& elem : data)
    {
      static const auto fgcolor = ImVec4(0.9F, 0.9F, 1.0, 1.0);
      static const auto bold_color = ImVec4(1.0, 0.5F, 0.5F, 1.0);
      static const auto disabled_color = ImVec4(0.6F, 0.6F, 0.6F, 1.0);

      auto color = (row % 4 == 0) ? bold_color : disabled_color;
      if (row == current_row)
      {
        ImGui::SetScrollHereY();
        color = fgcolor;

        if ((column - 1) == current_column)
        {
          draw_cursor(elem);
        }
      }
      ImGui::TextColored(color, "%s", elem.c_str());
      ++row;
    }
  }
  ImGui::EndChild();
  ImGui::PopStyleColor();
  ++column;
}

namespace {

  std::size_t subcolumn_to_strpos(int subcol)
  {
    switch (subcol)
    {
    case 0:
      return 3;
    case 1:
      return 4;
    case 2:
      return 5;
    case 3:
      return 7;
    case 4:
      return 8;
    case 5:
      return 10;
    case 6:
      return 11;
    case 7:
      return 12;
    case 8:
      return 13;
    }

    throw std::logic_error(fmt::format("subcol {} too high", subcol));
  }

} // anonymous namespace

void PatternView::draw_cursor(const std::string& elem) const
{
  auto cursorpos = ImGui::GetCursorScreenPos();
  auto textsize = ImGui::CalcTextSize(elem.c_str());
  draw_rect(cursorpos, textsize, 0xffffffff);

  auto strpos = subcolumn_to_strpos(current_subcolumn);
  if (current_subcolumn == 0)
  {
    textsize = ImGui::CalcTextSize(elem.substr(0, strpos).c_str());
  }
  else
  {
    textsize = ImGui::CalcTextSize(elem.substr(strpos, 1).c_str());
    cursorpos.x += ImGui::CalcTextSize(elem.substr(0, strpos).c_str()).x;
  }
  draw_rect(cursorpos, textsize, 0xffff0000);
}

void PatternView::draw_rect(const ImVec2& pos, const ImVec2& size,
                            std::uint32_t color) const
{
  ImVec2 minpos{pos.x - 1, pos.y - 1};
  ImVec2 maxpos{pos.x + size.x + 1, pos.y + size.y + 1};
  ImGui::GetForegroundDrawList()->AddRect(minpos, maxpos, color);
}
