#include "instrument_list.h"

#include "instrument.h"
#include "instrument_selection_listener.h"
#include "project.h"

#include <imgui.h>

InstrumentList::InstrumentList() :
  digg::SubWindow{"Instruments"}
{
}

void InstrumentList::set_current_project(Project& proj)
{
  project = &proj;
  if (current_item != 0)
  {
    current_item = 0;
    notify_listeners();
  }
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
    ImGui::ListBox("", &this->current_item, &empty, 1, height_in_items);
  }
  else
  {
    if (ImGui::ListBox("", &this->current_item, &items[0],
                       static_cast<int>(items.size()), height_in_items))
    {
      notify_listeners();
    }
  }
}

void InstrumentList::add_selection_listener(InstrumentSelectionListener& listener)
{
  listeners.insert(&listener);
  auto* instr = project->get_instrument(static_cast<std::size_t>(current_item));
  listener.selected_instrument_changed(instr);
}

void InstrumentList::remove_selection_listener(InstrumentSelectionListener& listener)
{
  listeners.erase(&listener);
}

void InstrumentList::notify_listeners() const
{
  auto* instr = project->get_instrument(static_cast<std::size_t>(current_item));
  for (auto* l : listeners)
  {
    l->selected_instrument_changed(instr);
  }
}
