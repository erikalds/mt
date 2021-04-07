add_executable(mtlib_tests
  note_test.cpp
  pattern_test.cpp
  sample_data_iterator_test.cpp
  wav_file_format_test.cpp)

target_link_libraries(mtlib_tests PRIVATE
  project_warnings project_options
  mtlib
  catch_main
  CONAN_PKG::fmt)

target_include_directories(mtlib_tests PRIVATE ${INTERNAL_INCLUDES})

catch_discover_tests(
  mtlib_tests
  TEST_PREFIX
  "unittests.mtlib."
  EXTRA_ARGS
  -s
  --reporter=xml
  --out=unittests.mtlib.xml)
