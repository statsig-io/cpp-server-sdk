#include <gtest/gtest.h>
#include <statsig.h>
#include <iostream>

#include "utils/httpfixture.h"

class LoggerFixtureBase : public HttpFixture
{
};

class ImmediateLoggerFixture : public LoggerFixtureBase
{
protected:
  ImmediateLoggerFixture() : LoggerFixtureBase()
  {
    this->options.loggingMaxBufferSize = 1; // Flush after every event
  }
};

class PeriodicLoggerFixture : public LoggerFixtureBase
{
protected:
  PeriodicLoggerFixture() : LoggerFixtureBase()
  {
    this->options.loggingIntervalMs = 100; // Flush every 100 ms
  }
};

TEST_F(ImmediateLoggerFixture, ImmediateFlush)
{
  bool passGate = statsig::checkGate(this->publicUser, "always_on_gate");
  EXPECT_TRUE(passGate);
  EXPECT_EQ(this->logEvents.size(), 1);

  bool failGate = statsig::checkGate(this->publicUser, "on_for_statsig_email");
  EXPECT_FALSE(failGate);
  EXPECT_EQ(this->logEvents.size(), 2);

  auto testConfig = statsig::getConfig(this->publicUser, "test_config");
  EXPECT_EQ(testConfig.name, "test_config");
  EXPECT_EQ(testConfig.ruleID, "default");
  std::unordered_map<std::string, JSON::any> expectedValue = {
      {"number", 4},
      {"string", "default"},
      {"boolean", true},
  };
  EXPECT_EQ(testConfig.value, expectedValue);
  EXPECT_EQ(this->logEvents.size(), 3);

  EXPECT_EQ(this->logEvents[0].eventName, "statsig::gate_exposure");
  EXPECT_EQ(this->logEvents[0].metadata["gate"], "always_on_gate");
  EXPECT_EQ(this->logEvents[0].metadata["gateValue"], "true");
  EXPECT_EQ(this->logEvents[0].user, this->publicUser);
}

TEST_F(PeriodicLoggerFixture, PeriodicFlush)
{
  statsig::checkGate(this->publicUser, "always_on_gate");
  EXPECT_EQ(this->logEvents.size(), 0);

  statsig::getConfig(this->publicUser, "test_config");
  EXPECT_EQ(this->logEvents.size(), 0);

  boost::this_thread::sleep_for(boost::chrono::milliseconds(100 + HttpFixture::TIME_BUFFER));
  EXPECT_EQ(this->logEvents.size(), 2);
}