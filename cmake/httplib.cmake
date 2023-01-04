set(HTTPLIB_REQUIRE_OPENSSL ON)

FetchContent_Declare(httplib
  GIT_REPOSITORY    https://github.com/yhirose/cpp-httplib.git
  GIT_TAG           v0.11.3
)

FetchContent_MakeAvailable(httplib)