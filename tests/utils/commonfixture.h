#pragma once

#include <gtest/gtest.h>
#include <statsig.h>

class CommonFixture : public ::testing::Test
{
protected:
  statsig::Statsig statsig;
  statsig::Options options;
  std::string sdkKey;
  CommonFixture() {
    sdkKey = std::getenv("test_api_key");
    options.localMode = true;
  }
  void SetUp() override
  {
    std::cout << "initializing with options " << options << std::endl;
    statsig.initialize(sdkKey, options);
  }
  void TearDown() override
  {
    statsig.shutdown();
  }
};
