#include "instrument_list.h"

#include "instrument_selection_listener.h"
#include "mtlib/project.h"
#include "mtlib/instrument.h"
#include "mtlib/sample.h"

#include <fmt/format.h>
#include <imgui.h>

InstrumentList::InstrumentList() :
  digg::SubWindow{"Instruments"}
{
}

void InstrumentList::set_current_project(mt::Project& proj)
{
  project = &proj;
  current_instrument = 0;
  notify_listeners();
}

namespace {

  void list_names(std::function<std::size_t()> size_fun,
                  std::function<std::string_view(std::size_t)> name_fun,
                  std::string_view label,
                  int& current_item,
                  const std::function<void()>& selection_changed_fun)
  {
    const int height_in_items{6};
    std::vector<const char*> items;
    items.reserve(size_fun());
    for (auto i = 0U; i < size_fun(); ++i)
    {
      items.push_back(name_fun(i).data());
    }
    if (items.empty())
    {
      const char* empty{"0: <empty>"};
      ImGui::ListBox(label.data(), &current_item, &empty, 1, height_in_items);
    }
    else
    {
      if (ImGui::ListBox(label.data(), &current_item, &items[0],
                         static_cast<int>(items.size()), height_in_items))
      {
        selection_changed_fun();
      }
    }
  }

} // anonymous namespace

void InstrumentList::draw_widgets()
{
  list_names([&]() { return project->instrument_count(); },
             [&](std::size_t i)
             {
               assert(project->get_instrument(i) != nullptr);
               return project->get_instrument(i)->name();
             },
             "Instruments", current_instrument,
             [&]()
             {
               notify_listeners();
             });
  auto* instr = project->get_instrument(static_cast<std::size_t>(current_instrument));
  list_names([instr]() { return instr == nullptr ? 0U : instr->sample_count(); },
             [instr](std::size_t i)
             {
               assert(instr != nullptr);
               return instr->sample(i)->name();
             },
             "Samples", current_sample,
             [&]()
             {
               notify_listeners();
             });
}

void InstrumentList::add_selection_listener(InstrumentSelectionListener& listener)
{
  listeners.insert(&listener);
  notify_listener(listener);
}

void InstrumentList::remove_selection_listener(InstrumentSelectionListener& listener)
{
  listeners.erase(&listener);
}

void InstrumentList::notify_listeners() const
{
  for (auto* l : listeners)
  {
    notify_listener(*l);
  }
}

void InstrumentList::notify_listener(InstrumentSelectionListener& listener) const
{
  auto* instr = project->get_instrument(static_cast<std::size_t>(current_instrument));
  mt::Sample* sample = nullptr;
  if (instr != nullptr)
  {
    sample = instr->sample(static_cast<std::size_t>(current_sample));
  }
  listener.selected_instrument_changed(instr, sample);
}
