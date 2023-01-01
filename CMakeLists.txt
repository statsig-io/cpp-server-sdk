cmake_minimum_required(VERSION 3.11)

project(Statsig
  VERSION "0.0.0"
  DESCRIPTION "Statsig C++ Server SDK"
)

# Contains various cmake target files for 3rd party libraries
set(CMAKE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Download 3rd party libraries.
include(FetchContent)
include(${CMAKE_FILES}/httplib.cmake)
include(${CMAKE_FILES}/json.cmake)

# Create object libraries so that they may be packaged
# into a single statsig library instead of being separate dependencies
# file(GLOB HTTPLIB_SRC ${httplib_SOURCE_DIR})
# file(GLOB JSON_SRC ${json_SOURCE_DIR})
# add_library(json-target OBJECT ${HTTPLIB_SRC})
# add_library(httplib-target OBJECT ${json_SOURCE_DIR})

# Define the main statsig target.
file(GLOB STATSIG_SOURCE_FILES "src/*")
add_library(Statsig
  # $<TARGET_OBJECTS:httplib-target>
  # $<TARGET_OBJECTS:json-target>
  ${STATSIG_SOURCE_FILES}
)

target_link_libraries(Statsig PRIVATE httplib::httplib)
target_link_libraries(Statsig PRIVATE nlohmann_json::nlohmann_json)

# Find 3rd party packages via CMake Find Modules
# These are usually included in CMake distribution
list(APPEND CMAKE_MODULE_PATH "${CMAKE_FILES}") # For any additional FindXXX.cmake files
find_package(Boost REQUIRED COMPONENTS regex)

# Allow linking against the target Statsig::cpp when using find_package, or when
# using add_subdirectory.
add_library(Statsig::cpp ALIAS Statsig)

target_include_directories(Statsig PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(Statsig PRIVATE ${Boost_LIBRARIES})
message(
"Boost package:
    found: ${Boost_FOUND}
    include dir: ${Boost_INCLUDE_DIRS}
    libraries: ${Boost_LIBRARIES}"
)

target_compile_features(Statsig PRIVATE cxx_std_17)