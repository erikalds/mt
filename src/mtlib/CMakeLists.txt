add_library(mtlib STATIC
  instrument.cpp
  instrument.h
  note.cpp
  note.h
  sample.cpp
  sample.h
  sample_data_iterator.h)

target_include_directories(mtlib PRIVATE ${INTERNAL_INCLUDES})

target_link_libraries(mtlib PRIVATE
  project_warnings project_options
  CONAN_PKG::imgui-sfml CONAN_PKG::spdlog)
