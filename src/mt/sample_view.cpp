#include "sample_view.h"

#include "mtlib/sample_data_iterator.h"
#include <fmt/format.h>
#include <imgui.h>
#include <cstdint>

SampleView::SampleView() :
  channels{}
{
}

void SampleView::paint()
{
  const auto plot_height{80};
  int i{0};
  for (const auto& c : channels)
  {
    auto label = fmt::format("Channel {}", ++i);
    ImGui::PlotLines(label.c_str(), &c[0], static_cast<int>(c.size()), 0,
                     overlay.c_str(), static_cast<float>(INT16_MIN),
                     static_cast<float>(INT16_MAX), ImVec2{0, plot_height});
  }
}

void SampleView::clear()
{
  channels.clear();
}

void SampleView::present_details(unsigned int sample_rate, const sf::Time& duration)
{
  overlay = fmt::format("{} s. / {} Hz", duration.asSeconds(), sample_rate);
}

void SampleView::present_channel(mt::sample_data_iterator<const std::int16_t> begin,
                                 mt::sample_data_iterator<const std::int16_t> end)
{
  std::vector<float> c{};
  c.reserve(std::distance(begin, end));
  std::transform(begin, end, std::back_inserter(c),
                 [](const std::int16_t& v) { return static_cast<float>(v); });
  channels.emplace_back(std::move(c));
}
