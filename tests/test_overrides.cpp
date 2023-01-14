#include <gtest/gtest.h>
#include <statsig.h>
#include <iostream>

#include "utils/commonfixture.h"

class OverridesFixture : public CommonFixture
{
};

TEST_F(OverridesFixture, GateOverrides)
{
  bool gateValue;
  gateValue = statsig.checkGate(this->publicUser, "test_email");
  EXPECT_FALSE(gateValue);
  statsig.overrideGate("test_email", true, this->publicUser.userID);
  gateValue = statsig.checkGate(this->publicUser, "test_email");
  EXPECT_TRUE(gateValue);
  statsig.removeGateOverride("test_email");
  gateValue = statsig.checkGate(this->publicUser, "test_email");
  EXPECT_FALSE(gateValue);
}

TEST_F(OverridesFixture, ConfigOverrides)
{
  statsig::DynamicConfig testConfig;
  testConfig = statsig.getConfig(this->publicUser, "test_custom_config");
  std::unordered_map<std::string, JSON::any> expectedValue = {
      {"header_text", "new user test"},
      {"foo", "bar"},
  };
  EXPECT_EQ(testConfig.value, expectedValue);
  std::unordered_map<std::string, JSON::any> overrideValue = {
      {"overridden key", "overridden field"},
  };
  statsig.overrideConfig("test_custom_config", overrideValue, this->publicUser.userID);
  testConfig = statsig.getConfig(this->publicUser, "test_custom_config");
  EXPECT_EQ(testConfig.value, overrideValue);
  statsig.removeConfigOverride("test_custom_config");
  testConfig = statsig.getConfig(this->publicUser, "test_custom_config");
  EXPECT_EQ(testConfig.value, expectedValue);
}