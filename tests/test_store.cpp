#include <gtest/gtest.h>
#include <statsig.h>
#include <iostream>

#include "utils/httpfixture.h"

class StoreFixture : public HttpFixture
{
protected:
  StoreFixture() : HttpFixture()
  {
    this->options.rulesetsSyncIntervalMs = 100;
  }
};

TEST_F(StoreFixture, IntervalSync)
{
  EXPECT_EQ(this->counter.downloadConfigSpecs, 1);
  boost::this_thread::sleep_for(boost::chrono::milliseconds(100 + HttpFixture::TIME_BUFFER));
  EXPECT_EQ(this->counter.downloadConfigSpecs, 2);
  boost::this_thread::sleep_for(boost::chrono::milliseconds(50 + HttpFixture::TIME_BUFFER));
  EXPECT_EQ(this->counter.downloadConfigSpecs, 2);
  boost::this_thread::sleep_for(boost::chrono::milliseconds(50 + HttpFixture::TIME_BUFFER));
  EXPECT_EQ(this->counter.downloadConfigSpecs, 3);
}