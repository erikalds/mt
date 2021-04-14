add_executable(digg_tests
  action_test.cpp
  event_processor_test.cpp)

target_link_libraries(digg_tests PRIVATE
  project_warnings project_options
  digg
  catch_main
  CONAN_PKG::imgui-sfml)

target_include_directories(digg_tests PRIVATE ${INTERNAL_INCLUDES})

catch_discover_tests(
  digg_tests
  TEST_PREFIX
  "unittests.digg."
  REPORTER
  xml
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "unittests.digg."
  OUTPUT_SUFFIX
  .xml)
