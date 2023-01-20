cmake_minimum_required(VERSION 3.11)

option(STATSIG_BUILD_TESTS "Compile with unit tests" OFF)
option(STATSIG_INCLUDE_MAXMINDDB "Include the 3rd party library maxminddb. (Disable if country lookup is not needed)" ON)

project(Statsig
  VERSION "0.0.0"
  DESCRIPTION "Statsig C++ Server SDK"
)

# Define the main statsig target.
file(GLOB STATSIG_SOURCE_FILES "src/*")
add_library(Statsig ${STATSIG_SOURCE_FILES})

# Contains various cmake target files and CMake Find files for 3rd party libraries
set(CMAKE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_FILES} CACHE PATH "Local CMAKE_MODULE_PATH")

# Default system variables needed for dependencies
# NOTE: These will likely need to be overriden when importing this library
set(OPENSSL_ROOT_DIR /opt/homebrew/opt/openssl)

# Download 3rd party libraries.
include(FetchContent)
include(${CMAKE_FILES}/json.cmake)
include(${CMAKE_FILES}/httplib.cmake)

target_link_libraries(Statsig PUBLIC httplib::httplib)
target_link_libraries(Statsig PUBLIC nlohmann_json::nlohmann_json)

# Find 3rd party packages via CMake Find Modules
# These are usually included in CMake distribution and don't need to be installed
find_package(Boost REQUIRED COMPONENTS regex chrono thread system)
target_include_directories(Statsig PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(Statsig PUBLIC ${Boost_LIBRARIES})
find_package(OpenSSL REQUIRED)
target_include_directories(Statsig PUBLIC ${OpenSSL_INCLUDE_DIRS})
target_link_libraries(Statsig PUBLIC ${OpenSSL_LIBRARIES})

# Try to find maxminddb, fallback to installing directly from github
if (STATSIG_INCLUDE_MAXMINDDB)
  find_package(MaxMindDB)
  if (DEFINED LIBMAXMINDDB_LIBRARY)
    message(STATUS "Found MaxMindDB: ${LIBMAXMINDDB_LIBRARY}")
    target_include_directories(Statsig PUBLIC ${LIBMAXMINDDB_INCLUDE_DIR})
    target_link_libraries(Statsig PUBLIC ${LIBMAXMINDDB_LIBRARY})
  else()
    message(STATUS "MaxMindDB not found. Installing from https://github.com/maxmind/libmaxminddb.git")
    include(${CMAKE_FILES}/maxminddb.cmake)
    target_link_libraries(Statsig PUBLIC maxminddb::maxminddb)
  endif()
  add_definitions( -DMAXMINDDB_DEFINED -DPROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )
  add_compile_definitions( _DARWIN_C_SOURCE ) # Necessary for setting certain types in MacOS systems.
endif()

# Path to header files
set(STATSIG_INCLUDE_PATHS 
  "src"
  ${Boost_INCLUDE_DIRS}
  ${OpenSSL_INCLUDE_DIRS}
  ${LIBMAXMINDDB_INCLUDE_DIR}
)

target_include_directories(Statsig 
  PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:src>
  PRIVATE
    ${STATSIG_INCLUDE_PATHS}
)

# Allow linking against the target Statsig::cpp when using find_package, or when
# using add_subdirectory.
add_library(Statsig::cpp ALIAS Statsig)

target_compile_features(Statsig PUBLIC cxx_std_17)

# Build the unit test binary, and add all of the unit tests.
if(STATSIG_BUILD_TESTS)
  add_subdirectory(tests)
endif()

add_definitions( -DPROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )
