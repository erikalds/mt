#include "pattern_queue_editor.h"

#include "mt/pattern_view.h"
#include "mtlib/pattern.h"
#include "mtlib/pattern_queue.h"
#include <fmt/format.h>
#include <imgui.h>

PatternQueueEditor::PatternQueueEditor(PatternView& pv) :
  digg::SubWindow("Pattern Queue"),
  pattern_view(pv)
{
}

void PatternQueueEditor::set_pattern_queue(mt::PatternQueue& pq)
{
  pattern_queue = &pq;
  current_queue_idx = 0;
  display_current_pattern();
}

namespace {

  std::string s = {}; // string s needs to outlive items_getter function

  bool items_getter(void* data, int idx, const char** out_text)
  {
    const auto* pq = static_cast<const mt::PatternQueue*>(data);
    const auto& pattern = pq->at(static_cast<std::size_t>(idx));
    s = fmt::format("{:2x}\t{}", idx, pattern.name());
    *out_text = s.c_str();
    return true;
  }

} // anonymous namespace

void PatternQueueEditor::draw_widgets()
{
  const auto height_in_items = 6;
  if (ImGui::ListBox("Patterns", &current_queue_idx, items_getter,
                     pattern_queue, static_cast<int>(pattern_queue->size()),
                     height_in_items))
  {
    display_current_pattern();
  }
  if (ImGui::Button("add"))
  {
    pattern_queue->append_duplicate_pattern(std::begin(*pattern_queue) + current_queue_idx);
    ++current_queue_idx;
    display_current_pattern();
  }
  ImGui::SameLine();
  if (ImGui::Button("rem"))
  {
    pattern_queue->remove_pattern_from_queue(std::begin(*pattern_queue) + current_queue_idx);
    current_queue_idx = std::min(current_queue_idx,
                                 static_cast<int>(pattern_queue->size()) - 1);
    current_queue_idx = std::max(current_queue_idx, 0);
    display_current_pattern();
  }
  ImGui::SameLine();
  if (ImGui::Button("inc"))
  {
    pattern_queue->increment_pattern_at(std::begin(*pattern_queue) + current_queue_idx);
    display_current_pattern();
  }
  ImGui::SameLine();
  if (ImGui::Button("dec"))
  {
    pattern_queue->decrement_pattern_at(std::begin(*pattern_queue) + current_queue_idx);
    display_current_pattern();
  }
}

void PatternQueueEditor::display_current_pattern()
{
  auto& pattern = pattern_queue->at(static_cast<std::size_t>(current_queue_idx));
  pattern_view.display_pattern(pattern);
}
