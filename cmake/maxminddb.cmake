cmake_policy(SET CMP0077 NEW)
SET(BUILD_TESTING OFF)
SET(BUILD_SHARED_LIBS ON)

FetchContent_Declare(maxminddb
  GIT_REPOSITORY    https://github.com/maxmind/libmaxminddb.git
  GIT_TAG           1.7.1
)

FetchContent_MakeAvailable(maxminddb)