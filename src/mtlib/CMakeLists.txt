add_library(mtlib STATIC
  instrument.cpp
  instrument.h
  note.cpp
  note.h
  project.cpp
  project.h
  sample.cpp
  sample.h
  sample_data_iterator.h)

target_include_directories(mtlib PRIVATE ${INTERNAL_INCLUDES})

target_link_libraries(mtlib PRIVATE
  project_warnings project_options
  base64
  CONAN_PKG::imgui-sfml CONAN_PKG::yaml-cpp CONAN_PKG::spdlog)
