#pragma once

#include <string>

namespace statsig
{
  const struct StatsigMetadata
  {
    std::string sdkType;
    std::string sdkVersion;
  } StatsigMetadata = {"cpp-sdk", "0.0.0"};
}