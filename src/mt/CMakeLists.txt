add_executable(mt
  instrument_editor.cpp
  instrument_editor.h
  instrument_list.cpp
  instrument_list.h
  keyboard.cpp
  keyboard.h
  main.cpp
  muzaktracker.cpp
  muzaktracker.h
  sample_view.cpp
  sample_view.h)

target_include_directories(mt PRIVATE ${INTERNAL_INCLUDES})

target_link_libraries(mt PRIVATE
  project_warnings project_options
  digg mtlib
  CONAN_PKG::imgui-sfml CONAN_PKG::fmt CONAN_PKG::spdlog CONAN_PKG::docopt.cpp)
