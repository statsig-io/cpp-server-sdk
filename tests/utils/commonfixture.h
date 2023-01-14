#pragma once

#include <gtest/gtest.h>
#include <statsig.h>

class CommonFixture : public ::testing::Test
{
protected:
  statsig::Statsig statsig;
  statsig::Options options;
  std::string sdkKey;
  statsig::User publicUser;
  statsig::User statsigUser;
  CommonFixture() {
    sdkKey = std::getenv("test_api_key");
    options.localMode = true;
    publicUser.userID = "public user";
    publicUser.email = "testuser@notstatsig.com";
    statsigUser.userID = "statsig user";
    statsigUser.email = "testuser@statsig.com";
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
