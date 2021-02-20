add_library(digg STATIC
  action.cpp
  action.h
  event_processor.cpp
  event_processor.h
  event_listener.h
  mainmenubar.cpp
  mainmenubar.h
  mainwindow.cpp
  mainwindow.h
  menubar.cpp
  menubar.h
  menuitem.cpp
  menuitem.h
  menu.cpp
  menu.h
  sentry.h
  subwindow.cpp
  subwindow.h
  widget.h)

target_include_directories(digg PRIVATE ${INTERNAL_INCLUDES})

target_link_libraries(digg PRIVATE
  project_warnings project_options
  CONAN_PKG::imgui-sfml CONAN_PKG::spdlog)
