add_executable(sndmix_tests
  interpolating_sample_data_iterator_test.cpp
  sample_data_iterator_test.cpp
  sinknode_test.cpp
  soundbuffer_test.cpp
  sound_test.cpp
  )

target_link_libraries(sndmix_tests PRIVATE
  project_warnings project_options
  sndmix
  catch_main
  CONAN_PKG::fmt)

target_include_directories(sndmix_tests PRIVATE ${INTERNAL_INCLUDES})

catch_discover_tests(
  sndmix_tests
  TEST_PREFIX
  "unittests.sndmix."
  REPORTER
  xml
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "unittest.sndmix."
  OUTPUT_SUFFIX
  .xml)
