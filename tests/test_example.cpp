#include <gtest/gtest.h>
#include <statsig.h>
#include <iostream>

#include "utils/commonfixture.h"

class ExampleFixture : public CommonFixture
{
};

TEST_F(ExampleFixture, BasicAPI)
{
  std::cout << "Running test" << std::endl;
  bool passGate = statsig::checkGate(this->publicUser, "test_public");
  EXPECT_TRUE(passGate);

  bool failGate = statsig::checkGate(this->publicUser, "test_email");
  EXPECT_FALSE(failGate);

  statsig::DynamicConfig testConfig = statsig::getConfig(this->publicUser, "test_custom_config");
  EXPECT_EQ(testConfig.name, "test_custom_config");
  EXPECT_EQ(testConfig.ruleID, "default");
  std::unordered_map<std::string, JSON::any> expectedValue = {
      {"header_text", "new user test"},
      {"foo", "bar"},
  };
  EXPECT_EQ(testConfig.value, expectedValue);
}