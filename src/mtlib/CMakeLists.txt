add_library(mtlib STATIC
  audio_data_presenter.h
  config.cpp
  config.h
  instrument.cpp
  instrument.h
  note.cpp
  note.h
  pattern.cpp
  pattern.h
  pattern_queue.cpp
  pattern_queue.h
  project.cpp
  project.h
  sample.cpp
  sample.h
  wav_file_format.cpp
  wav_file_format.h)

target_include_directories(mtlib PRIVATE ${INTERNAL_INCLUDES})

target_link_libraries(mtlib PRIVATE
  project_warnings project_options
  base64 sndmix
  CONAN_PKG::imgui-sfml CONAN_PKG::yaml-cpp CONAN_PKG::spdlog)
