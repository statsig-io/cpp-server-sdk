set(HTTPLIB_USE_OPENSSL_IF_AVAILABLE OFF)
set(HTTPLIB_COMPILE ON)

FetchContent_Declare(httplib
        GIT_REPOSITORY    https://github.com/yhirose/cpp-httplib.git
        GIT_TAG           v0.11.3
)

FetchContent_MakeAvailable(httplib)