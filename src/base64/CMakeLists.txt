add_library(base64 STATIC
  decode.cpp
  decode.h
  encode.cpp
  encode.h
  mask.h)

target_link_libraries(base64 PRIVATE
  project_warnings project_options)
