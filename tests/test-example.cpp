#include <gtest/gtest.h>
#include <statsig.h>
#include <iostream>

#include "utils/commonfixture.h"

class EvaluatorFixture : public CommonFixture
{
};

TEST_F(EvaluatorFixture, DefaultOptions)
{
  statsig::User user;
  user.userID = "test user";
  user.email = "testuser@notstatsig.com";

  bool passGate = statsig.checkGate(user, "test_public");
  EXPECT_TRUE(passGate);

  bool failGate = statsig.checkGate(user, "test_email");
  EXPECT_FALSE(failGate);

  statsig::DynamicConfig testConfig = statsig.getConfig(user, "test_custom_config");
  EXPECT_EQ(testConfig.name, "test_custom_config");
  EXPECT_EQ(testConfig.ruleID, "default");
  std::unordered_map<std::string, JSON::any> expectedValue = {
      {"header_text", "new user test"},
      {"foo", "bar"},
  };
  EXPECT_EQ(testConfig.value, expectedValue);

  statsig.shutdown();
}