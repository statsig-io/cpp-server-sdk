#pragma once

#include <string>

namespace statsig
{
  const struct StatsigMetadata
  {
    std::string sdkType;
    std::string sdkVersion;
    inline std::string operator()(struct StatsigMetadata const &obj)
    {
      return obj.sdkType + ":" + obj.sdkVersion;
    };
  } StatsigMetadata = {"cpp-sdk", "0.0.0"};

  inline bool operator==(struct StatsigMetadata const &a, struct StatsigMetadata const &b)
  {
    return a.sdkType == b.sdkType && a.sdkVersion == b.sdkVersion;
  };
  inline bool operator!=(struct StatsigMetadata const &a, struct StatsigMetadata const &b)
  {
    return a.sdkType != b.sdkType || a.sdkVersion != b.sdkVersion;
  };
}