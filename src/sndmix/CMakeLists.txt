add_library(sndmix STATIC
  audiochannel.h
  audiomixer.cpp
  audiomixer.h
  audiostream.cpp
  audiostream.h
  audiosystem.cpp
  audiosystem.h
  channel.cpp
  channel.h
  interpolating_sample_data_iterator.h
  mixer.cpp
  mixer.h
  node.h
  sample_data_iterator.h
  sinknode.cpp
  sinknode.h
  soundbuffer.cpp
  soundbuffer.h
  sound.cpp
  sound.h
  sourcenode.cpp
  sourcenode.h
  )

target_include_directories(sndmix PRIVATE ${INTERNAL_INCLUDES})

target_link_libraries(sndmix PRIVATE
  project_warnings project_options
  PortAudio::PortAudio
  CONAN_PKG::fmt CONAN_PKG::spdlog)
