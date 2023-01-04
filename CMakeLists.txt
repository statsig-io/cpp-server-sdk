cmake_minimum_required(VERSION 3.11)

project(Statsig
  VERSION "0.0.0"
  DESCRIPTION "Statsig C++ Server SDK"
)

# Define the main statsig target.
file(GLOB STATSIG_SOURCE_FILES "src/*")
add_library(Statsig ${STATSIG_SOURCE_FILES})

# Path to header files
set(STATSIG_INCLUDE_PATHS "src")

target_include_directories(Statsig 
  PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:src>
  PRIVATE
    ${STATSIG_INCLUDE_PATHS}
)

# Contains various cmake target files for 3rd party libraries
set(CMAKE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Download 3rd party libraries.
include(FetchContent)
include(${CMAKE_FILES}/json.cmake)
include(${CMAKE_FILES}/httplib.cmake)

target_link_libraries(Statsig PUBLIC httplib::httplib)
target_link_libraries(Statsig PUBLIC nlohmann_json::nlohmann_json)

# Find 3rd party packages via CMake Find Modules
# These are usually included in CMake distribution and don't need to be installed
find_package(Boost REQUIRED)
target_include_directories(Statsig PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(Statsig PUBLIC ${Boost_LIBRARIES})

# Allow linking against the target Statsig::cpp when using find_package, or when
# using add_subdirectory.
add_library(Statsig::cpp ALIAS Statsig)

target_compile_features(Statsig PUBLIC cxx_std_17)

add_executable(test_statsig ${CMAKE_CURRENT_SOURCE_DIR}/tests/test-initialize.cpp)
target_compile_features(test_statsig PRIVATE cxx_std_17)
target_link_libraries(test_statsig PRIVATE Statsig)
