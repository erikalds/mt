add_executable(mt
  main.cpp
  muzaktracker.cpp
  muzaktracker.h)

target_include_directories(mt PRIVATE ${INTERNAL_INCLUDES})

target_link_libraries(mt PRIVATE
  project_warnings project_options
  digg
  CONAN_PKG::imgui-sfml CONAN_PKG::fmt CONAN_PKG::spdlog CONAN_PKG::docopt.cpp)
