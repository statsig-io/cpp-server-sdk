#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

#include "network.h"
#include "types.h"
#include "utils.h"

namespace statsig
{
  class Store
  {
  public:
    Store(Network *network, Options options) : network(network), options(options){};
    std::optional<ConfigSpec> getGate(std::string gateName);
    std::optional<ConfigSpec> getConfig(std::string configName);
    std::optional<ConfigSpec> getLayer(std::string layerName);
    std::optional<std::string> getExperimentLayer(std::string experimentName);

  private:
    Network *network;
    Options options;
    std::unordered_map<std::string, ConfigSpec> featureGates;
    std::unordered_map<std::string, ConfigSpec> dynamicConfigs;
    std::unordered_map<std::string, ConfigSpec> layerConfigs;
    std::unordered_map<std::string, std::string> experimentToLayer;
    void fetchConfigSpecs();
    void processSpecsJSON(json specsJSON);
  };
}