add_executable(mtlib_tests
  note_test.cpp
  pattern_queue_test.cpp
  pattern_test.cpp
  wav_file_format_test.cpp)

target_link_libraries(mtlib_tests PRIVATE
  CONAN_PKG::catch2
  project_warnings project_options
  mtlib
  CONAN_PKG::fmt CONAN_PKG::yaml-cpp)

target_include_directories(mtlib_tests PRIVATE ${INTERNAL_INCLUDES})

catch_discover_tests(
  mtlib_tests
  TEST_PREFIX
  "unittests.mtlib."
  REPORTER
  XML
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "unittests.mtlib."
  OUTPUT_SUFFIX
  .xml)
