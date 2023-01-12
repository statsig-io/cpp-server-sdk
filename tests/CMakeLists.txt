cmake_minimum_required(VERSION 3.11)

include(GoogleTest)
include(FetchContent)
include(${CMAKE_FILES}/googletest.cmake)
enable_testing()

file(GLOB STATSIG_TEST_FILES "${PROJECT_SOURCE_DIR}/tests/*")

add_executable(google_tests
  ${STATSIG_SOURCE_FILES}
  ${STATSIG_TEST_FILES}
)
target_include_directories(google_tests PRIVATE ${STATSIG_INCLUDE_PATHS})
target_link_libraries(google_tests Statsig::cpp)
target_link_libraries(google_tests GTest::gtest_main)
target_compile_features(google_tests PUBLIC cxx_std_17)

add_definitions( -DTESTS_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

gtest_discover_tests(google_tests)