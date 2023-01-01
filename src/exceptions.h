#pragma once

#include <stdexcept>

namespace statsig
{
  class StatsigNotInitializedError : public std::runtime_error
  {
  public:
    StatsigNotInitializedError() : runtime_error("Statsig is not yet initialized"){};
  };
}