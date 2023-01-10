#pragma once

#include <gtest/gtest.h>
#include <statsig.h>

class CommonFixture : public ::testing::Test
{
protected:
  statsig::Statsig statsig;
  statsig::Options options;
  std::string sdkKey;
  void SetUp() override
  {
    sdkKey = std::getenv("test_api_key");
    statsig.initialize(sdkKey, options);
  }
  void TearDown() override
  {
    statsig.shutdown();
  }
};
