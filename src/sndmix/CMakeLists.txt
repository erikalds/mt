add_library(sndmix STATIC
  audiomixer.cpp
  audiomixer.h
  audiostream.cpp
  audiostream.h
  audiosystem.cpp
  audiosystem.h
  channel.cpp
  channel.h
  node.h
  sinknode.cpp
  sinknode.h
  soundbuffer.cpp
  soundbuffer.h
  sourcenode.cpp
  sourcenode.h
  )

target_include_directories(sndmix PRIVATE ${INTERNAL_INCLUDES})

target_link_libraries(sndmix PRIVATE
  project_warnings project_options
  portaudio
  CONAN_PKG::fmt CONAN_PKG::spdlog)
