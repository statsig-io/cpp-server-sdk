#include <gtest/gtest.h>
#include <statsig.h>
#include <iostream>

#include "utils/httpfixture.h"

class CountryLookupFixture : public HttpFixture
{
  protected:
  CountryLookupFixture() : HttpFixture()
  {
    this->publicUser.ipAddress = "24.18.183.148"; // Seattle, WA (US)
    this->statsigUser.ipAddress = "115.240.90.163"; // Mumbai, India (IN)
  }
};

TEST_F(CountryLookupFixture, TestCountry)
{
  // test_country is set to pass for US and CA
  bool passGate = statsig.checkGate(this->publicUser, "test_country");
  EXPECT_TRUE(passGate);
  bool failGate = statsig.checkGate(this->statsigUser, "test_country");
  EXPECT_FALSE(failGate);
}