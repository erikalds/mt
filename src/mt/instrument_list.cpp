#include "instrument_list.h"

#include "instrument_selection_listener.h"
#include "mtlib/project.h"
#include "mtlib/instrument.h"
#include "mtlib/sample.h"

#include <imgui.h>

InstrumentList::InstrumentList() :
  digg::SubWindow{"Instruments"}
{
}

void InstrumentList::set_current_project(mt::Project& proj)
{
  project = &proj;
  current_item = 0;
  notify_listeners();
}

void InstrumentList::draw_widgets()
{
  std::vector<const char*> items;
  const auto N = project->instrument_count();
  items.reserve(N);
  for (auto i = 0U; i < N; ++i)
  {
    items.push_back(project->get_instrument(i)->name().data());
  }

  const int height_in_items{6};
  if (items.empty())
  {
    const char* empty{"<empty>"};
    ImGui::ListBox("Instruments", &this->current_item, &empty, 1, height_in_items);
    ImGui::ListBox("Samples", &this->current_sample, &empty, 1, height_in_items);
  }
  else
  {
    if (ImGui::ListBox("Instruments", &this->current_item, &items[0],
                       static_cast<int>(items.size()), height_in_items))
    {
      notify_listeners();
    }
    const auto* instr = project->get_instrument(static_cast<std::size_t>(this->current_item));
    std::vector<const char*> sample_items;
    sample_items.reserve(instr->sample_count());
    for (auto i = 0U; i < instr->sample_count(); ++i)
    {
      sample_items.push_back(instr->sample(i)->name().data());
    }
    if (ImGui::ListBox("Samples", &this->current_sample, &sample_items[0],
                       static_cast<int>(sample_items.size()), height_in_items))
    {
      notify_listeners();
    }
  }
}

void InstrumentList::add_selection_listener(InstrumentSelectionListener& listener)
{
  listeners.insert(&listener);
  auto* instr = project->get_instrument(static_cast<std::size_t>(current_item));
  mt::Sample* sample = nullptr;
  if (instr != nullptr)
  {
    sample = instr->sample(static_cast<std::size_t>(current_sample));
  }
  listener.selected_instrument_changed(instr, sample);
}

void InstrumentList::remove_selection_listener(InstrumentSelectionListener& listener)
{
  listeners.erase(&listener);
}

void InstrumentList::notify_listeners() const
{
  auto* instr = project->get_instrument(static_cast<std::size_t>(current_item));
  mt::Sample* sample = nullptr;
  if (instr != nullptr)
  {
    sample = instr->sample(static_cast<std::size_t>(current_sample));
  }
  for (auto* l : listeners)
  {
    l->selected_instrument_changed(instr, sample);
  }
}
