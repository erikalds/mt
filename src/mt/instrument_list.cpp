#include "instrument_list.h"

#include "instrument_selection_listener.h"
#include "mtlib/project.h"
#include "mtlib/instrument.h"
#include "mtlib/sample.h"
#include "digg/file_dialog.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui_internal.h>


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
    std::vector<std::string> str_items;
    std::vector<const char*> items;
    str_items.reserve(size_fun());
    items.reserve(size_fun());
    for (auto i = 0U; i < size_fun(); ++i)
    {
      str_items.push_back(fmt::format("{}: {}", i, name_fun(i)));
      items.push_back(str_items.back().c_str());
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
  if (ImGui::Button("Add instrument"))
  {
    add_instrument();
  }
  ImGui::SameLine();
  ImGui::PushItemFlag(ImGuiItemFlags_Disabled,
                      project->instrument_count() == 0);
  if (ImGui::Button("Remove instrument"))
  {
    remove_current_instrument();
  }
  ImGui::PopItemFlag();

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

  auto* instr = this->get_current_instrument();
  ImGui::PushItemFlag(ImGuiItemFlags_Disabled, instr == nullptr);
  if (ImGui::Button("Add sample"))
  {
    add_sample();
  }
  ImGui::SameLine();
  ImGui::PushItemFlag(ImGuiItemFlags_Disabled,
                      instr == nullptr || instr->sample_count() == 0);
  if (ImGui::Button("Remove sample"))
  {
    remove_current_sample();
  }
  ImGui::PopItemFlag();
  ImGui::PopItemFlag();

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

  if (file_dialog != nullptr)
  {
    file_dialog->process();
  }
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

const mt::Instrument* InstrumentList::get_current_instrument() const
{
  return project->get_instrument(static_cast<std::size_t>(current_instrument));
}

mt::Instrument* InstrumentList::get_current_instrument()
{
  return project->get_instrument(static_cast<std::size_t>(current_instrument));
}

void InstrumentList::add_instrument()
{
  mt::Instrument new_instr{"untitled"};
  project->add_instrument(std::move(new_instr));
  current_instrument = static_cast<int>(project->instrument_count()) - 1;
  notify_listeners();
}

void InstrumentList::remove_current_instrument()
{
  for (auto* l : listeners)
  {
    l->selected_instrument_changed(0, nullptr, nullptr);
  }
  project->remove_instrument(static_cast<std::size_t>(current_instrument));
  current_instrument = std::max(0, std::min(current_instrument,
                                            static_cast<int>(project->instrument_count()) - 1));
  notify_listeners();
}

void InstrumentList::add_sample()
{
  file_dialog = std::make_unique<digg::FileDialog>("Open sample",
                                                   "*.wav",
                                                   [&](const std::filesystem::path& fname)
                                                   {
                                                     this->file_dialog->toggle_active();
                                                     auto* instr = this->get_current_instrument();
                                                     if (instr == nullptr)
                                                     {
                                                       return;
                                                     }

                                                     instr->add_sample(mt::Sample{fname});
                                                     this->notify_listeners();
                                                   },
                                                   [&]()
                                                   {
                                                     this->file_dialog->toggle_active();
                                                   });
}

void InstrumentList::remove_current_sample()
{
  auto* instr = this->get_current_instrument();
  if (instr == nullptr)
  {
    return;
  }

  for (auto* l : listeners)
  {
    l->selected_instrument_changed(current_instrument, instr, nullptr);
  }
  instr->remove_sample(static_cast<std::size_t>(current_sample));
  current_sample = std::max(0, std::min(current_sample,
                                        static_cast<int>(instr->sample_count()) - 1));
  notify_listeners();
}

void InstrumentList::notify_listeners()
{
  for (auto* l : listeners)
  {
    notify_listener(*l);
  }
}

void InstrumentList::notify_listener(InstrumentSelectionListener& listener)
{
  auto* instr = this->get_current_instrument();
  mt::Sample* sample = nullptr;
  if (instr != nullptr)
  {
    sample = instr->sample(static_cast<std::size_t>(current_sample));
  }
  listener.selected_instrument_changed(current_instrument, instr, sample);
}
