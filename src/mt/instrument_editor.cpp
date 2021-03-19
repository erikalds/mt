#include "instrument_editor.h"

#include "instrument.h"
#include "mtlib/sample.h"
#include <cstring>
#include <imgui.h>
#include <array>

InstrumentEditor::InstrumentEditor() :
  digg::SubWindow{"Instrument Editor"}
{
}

void InstrumentEditor::selected_instrument_changed(Instrument* i, mt::Sample* s)
{
  instrument = i;
  sample = s;
  if (sample)
  {
    //sample->
  }
}

namespace
{

  template<typename T, std::size_t bufsize=200>
  void input_text(T* thingy, const char* label)
  {
    std::string_view name{"<none selected>"};
    auto flags = ImGuiInputTextFlags_ReadOnly;
    if (thingy != nullptr)
    {
      name = thingy->name();
      flags = ImGuiInputTextFlags_None;
    }
    std::array<char, bufsize> buf{};
    std::strncpy(&buf[0], name.data(),
                 std::min(name.size(), buf.size()));
    if (ImGui::InputText(label, &buf[0], buf.size(), flags))
    {
      thingy->set_name(std::string_view{&buf[0]});
    }
  }

}  // anonymous namespace

void InstrumentEditor::draw_widgets()
{
  input_text(instrument, "Name");
  input_text(sample, "Sample Name");

  //ImGui::PlotLines("Wave-form",
}
