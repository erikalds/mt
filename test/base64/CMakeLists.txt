add_executable(base64_tests
  decode_test.cpp
  encode_test.cpp)

target_link_libraries(base64_tests PRIVATE
  project_warnings project_options
  base64
  catch_main)

target_include_directories(base64_tests PRIVATE ${INTERNAL_INCLUDES})

catch_discover_tests(
  base64_tests
  TEST_PREFIX
  "unittests.base64."
  REPORTER
  xml
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "unittests.base64."
  OUTPUT_SUFFIX
  .xml)
