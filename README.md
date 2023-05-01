# Statsig C++ Server SDK

The Statsig C++ SDK for multi-user, server side environments. If you need a SDK for another language or single user client environment, check out our [other SDKs](https://docs.statsig.com/#sdks).

Statsig helps you move faster with Feature Gates (Feature Flags) and Dynamic Configs. It also allows you to run A/B tests to validate your new features and understand their impact on your KPIs. If you're new to Statsig, create an account at [statsig.com](https://www.statsig.com).

## Getting Started

### __CMake__
`1.` Create a `.cmake` file that downloads the SDK at build time.
```
FetchContent_Declare(statsig
        GIT_REPOSITORY    https://github.com/statsig-io/private-cpp-server-sdk.git
        GIT_TAG           main
)

FetchContent_MakeAvailable(statsig)
```
`2.` Include the `.cmake ` file in your `CMakeLists.txt`
```
cmake_minimum_required(VERSION 3.11)

include(FetchContent)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/statsig.cmake)
```
`3.` Configure build options (optional)
```
option(STATSIG_BUILD_TESTS "Compile with unit tests" OFF)
option(STATSIG_INCLUDE_MAXMINDDB "Include the 3rd party library maxminddb. (Disable if country lookup is not needed)" ON)
```

## Testing

### __CMake__
You can set the option `set(STATSIG_BUILD_TESTS ON)` in your `CMakeLists.txt` to include tests in your build. Our tests are built using `GoogleTest`. To run the tests, use the command `ctest --test-dir build/tests`

See the [ctest manual](https://cmake.org/cmake/help/latest/manual/ctest.1.html) for more options.

## Guidelines

- Pull requests are welcome! 
- If you encounter bugs, feel free to [file an issue](https://github.com/statsig-io/cpp-server-sdk/issues).
- For integration questions/help, [join our slack community](https://join.slack.com/t/statsigcommunity/shared_invite/zt-pbp005hg-VFQOutZhMw5Vu9eWvCro9g).
