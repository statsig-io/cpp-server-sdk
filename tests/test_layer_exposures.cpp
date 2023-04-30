#include <gtest/gtest.h>
#include <statsig.h>
#include <iostream>

#include "utils/httpfixture.h"

class LayerExposuresFixture : public HttpFixture
{
  protected:
  LayerExposuresFixture() : HttpFixture()
  {
    this->dcsFilePath = "/testdata/layer_exposures_download_config_specs.json";
    this->options.loggingMaxBufferSize = 1;
  }
};

TEST_F(LayerExposuresFixture, DoesNotLogOnGetLayer)
{
  statsig::Layer layer = statsig::getLayer(this->publicUser, "unallocated_layer");
  EXPECT_EQ(this->logEvents.size(), 0);
}

TEST_F(LayerExposuresFixture, DoesNotLogOnInvalidType)
{
  statsig::Layer layer = statsig::getLayer(this->publicUser, "unallocated_layer");
  layer.get("an_int", std::string());
  EXPECT_EQ(this->logEvents.size(), 0);
}

TEST_F(LayerExposuresFixture, DoesNotLogOnNonExistentKey)
{
  statsig::Layer layer = statsig::getLayer(this->publicUser, "unallocated_layer");
  layer.get("a_string", std::string());
  EXPECT_EQ(this->logEvents.size(), 0);
}

TEST_F(LayerExposuresFixture, UnallocatedLayerLogging)
{
  statsig::Layer layer = statsig::getLayer(this->publicUser, "unallocated_layer");
  layer.get("an_int", 0);
  EXPECT_EQ(this->logEvents.size(), 1);
  EXPECT_EQ(this->logEvents[0].eventName, "statsig::layer_exposure");
  EXPECT_EQ(this->logEvents[0].metadata["config"], "unallocated_layer");
  EXPECT_EQ(this->logEvents[0].metadata["ruleID"], "default");
  EXPECT_EQ(this->logEvents[0].metadata["allocatedExperiment"], "");
  EXPECT_EQ(this->logEvents[0].metadata["parameterName"], "an_int");
  EXPECT_EQ(this->logEvents[0].metadata["isExplicitParameter"], "false");
  EXPECT_EQ(this->logEvents[0].user, this->publicUser);
}

TEST_F(LayerExposuresFixture, ExplicitVsImplicitParameterLogging)
{
  statsig::Layer layer = statsig::getLayer(this->publicUser, "explicit_vs_implicit_parameter_layer");
  layer.get("an_int", 0);
  layer.get("a_string", std::string());
  EXPECT_EQ(this->logEvents.size(), 2);
  EXPECT_EQ(this->logEvents[0].metadata["config"], "explicit_vs_implicit_parameter_layer");
  EXPECT_EQ(this->logEvents[0].metadata["ruleID"], "alwaysPass");
  EXPECT_EQ(this->logEvents[0].metadata["allocatedExperiment"], "experiment");
  EXPECT_EQ(this->logEvents[0].metadata["parameterName"], "an_int");
  EXPECT_EQ(this->logEvents[0].metadata["isExplicitParameter"], "true");
  EXPECT_EQ(this->logEvents[1].metadata["config"], "explicit_vs_implicit_parameter_layer");
  EXPECT_EQ(this->logEvents[1].metadata["ruleID"], "alwaysPass");
  EXPECT_EQ(this->logEvents[1].metadata["allocatedExperiment"], "");
  EXPECT_EQ(this->logEvents[1].metadata["parameterName"], "a_string");
  EXPECT_EQ(this->logEvents[1].metadata["isExplicitParameter"], "false");
}