add_executable(mt
  instrument.cpp
  instrument.h
  instrument_list.cpp
  instrument_list.h
  keyboard.cpp
  keyboard.h
  main.cpp
  muzaktracker.cpp
  muzaktracker.h
  note.cpp
  note.h
  project.cpp
  project.h)

target_include_directories(mt PRIVATE ${INTERNAL_INCLUDES})

target_link_libraries(mt PRIVATE
  project_warnings project_options
  digg
  CONAN_PKG::imgui-sfml CONAN_PKG::fmt CONAN_PKG::spdlog CONAN_PKG::docopt.cpp)
