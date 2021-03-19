add_executable(mtlib_tests
  sample_data_iterator_test.cpp)

target_link_libraries(mtlib_tests PRIVATE
  project_warnings project_options
  catch_main)

target_include_directories(mtlib_tests PRIVATE ${INTERNAL_INCLUDES})

catch_discover_tests(
  mtlib_tests
  TEST_PREFIX
  "unittests.mtlib."
  EXTRA_ARGS
  -s
  --reporter=xml
  --out=unittests.mtlib.xml)
