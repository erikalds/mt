#include "pattern_view.h"

#include "mtlib/pattern.h"
#include <fmt/core.h>
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
  const auto col_width{200};
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
      --current_column;
    }
    else if (e.key.code == sf::Keyboard::Right)
    {
      ++current_column;
    }
    else
    {
      return;
    }
    current_row = current_row % current_pattern->get_track_length();
    current_column = current_column % current_pattern->size();
    spdlog::debug("[Pattern] current column/row: {}/{}", current_column, current_row);
  }
  else if (e.type == sf::Event::KeyReleased)
  {
    spdlog::debug("[Pattern] key released: {}", e.key.code);
  }
}


// namespace ImGuiExt {

//   int BeginTable(const char* columnsId, const char** headers, float* widths,
//                  int count, bool draw_border)
//   {
//     if (count <= 0)
//     {
//       return 0;
//     }

//     // Draw column headers
//     ImGuiStyle& style = ImGui::GetStyle();
//     const ImVec2 firstTextSize = ImGui::CalcTextSize(headers[0], nullptr, true);

//     ImGui::BeginChild(columnsId, ImVec2(0, firstTextSize.y + 2 * style.ItemSpacing.y), true,
//                       ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

//     char str_id[256];
//     sprintf(str_id, "tbl0_%s", columnsId);
//     ImGui::Columns(count, str_id, draw_border);

//     float offset = 0.0f;
//     for(int i = 0; i < count; i++)
//     {
//       ImGui::SetColumnOffset(i, offset);

//       if (widths[i] <= 0)
//       {
//         const ImVec2 textsize = ImGui::CalcTextSize(headers[i], nullptr, true);
//         const float colSizeX = (textsize.x + 2 * style.ItemSpacing.x);
//         widths[i] = colSizeX + 1;
//       }

//       offset += widths[i];

//       // if (i < (count - 1))
//       // {
//       //   float curOffset = offset;
//       //   offset = ImGui::GetColumnOffset(i + 1);
//       //   widths[i] = offset - curOffset + 1;
//       // }

//       ImGui::SetColumnWidth(i, widths[i]);
//       ImGui::TextUnformatted(headers[i]);
//       ImGui::NextColumn();
//     }

//     ImGui::Columns(1);
//     ImGui::EndChild();

//     // Draw body
//     str_id[3] = '1';
//     columnsId = str_id;

//     ImGui::BeginChild(columnsId, ImVec2(0,0), true);
//     ImGui::Columns(count, columnsId, draw_border);

//     offset = 0.0f;
//     for(int i = 0; i < count; i++)
//     {
//       ImGui::SetColumnOffset(i, offset);
//       ImGui::SetColumnWidth(i, widths[i]);
//       offset += widths[i];// - 1;
//     }

//     return 1;
//   }

//   void EndTable()
//   {
//     ImGui::Columns(1);
//     ImGui::EndChild();
//   }

// }


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


  // if (ImGuiExt::BeginTable("tracks", &headers[0], &column_widths[0],
  //                          static_cast<int>(current_pattern->size() + 1), true) != 0)
  // {

  //   for (auto row = 0U; row < current_pattern->get_track_length(); ++row)
  //   {
  //     ImGui::Text("%.2x", row);
  //     //spdlog::debug("Column {} has width: {}", ImGui::GetColumnIndex(), ImGui::GetColumnWidth(ImGui::GetColumnIndex()));
  //     ImGui::NextColumn();
  //     for (const auto& track : *current_pattern)
  //     {
  //       //spdlog::debug("Column {} has width: {}", ImGui::GetColumnIndex(), ImGui::GetColumnWidth(ImGui::GetColumnIndex()));
  //       const auto repr = track[row].represent();
  //       ImGui::TextUnformatted(repr.c_str());
  //       ImGui::NextColumn();
  //     }
  //   }
  // }
  // ImGuiExt::EndTable();
}

void PatternView::render_column_header(std::size_t length)
{
  for (auto i = 0U; i < length; ++i)
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
    const ImVec2 childsize{column_widths[i], ImGui::GetTextLineHeight() * 2.0F};
    if (ImGui::BeginChild(ost.str().c_str(), childsize, true, flags))
    {
      ImGui::TextColored(ImVec4(1.0F, 1.0F, 1.0F, 1.0F), "%s", headers[i]);
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
    ImGui::PushStyleColor(ImGuiCol_ChildBg, 0x80444444);
  }
  else
  {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, 0x80222222);
  }

  std::ostringstream ost;
  ost << "Track-" << column;
  const ImVec2 childsize{column_widths[column], 0};
  if (ImGui::BeginChild(ost.str().c_str(), childsize, true, flags))
  {
    auto row = 0U;
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
      }
      ImGui::TextColored(color, "%s", elem.c_str());
      ++row;
    }
  }
  ImGui::EndChild();
  ImGui::PopStyleColor();
  ++column;
}
