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
    Store(Network *network, Options options)
    {
      this->network = network;
      this->options = options;
      this->featureGates = std::unordered_map<std::string, ConfigSpec>();
      this->dynamicConfigs = std::unordered_map<std::string, ConfigSpec>();
      this->layerConfigs = std::unordered_map<std::string, ConfigSpec>();
      this->experimentToLayer = std::unordered_map<std::string, std::string>();
      this->lastConfigSyncTime = 0;
      if (!options.localMode)
      {
        this->syncBgThread = Utils::spawnBackgroundThread([this]()
                                                          { sync(); });
      }
    };
    ~Store() { shutdown(); };
    std::optional<ConfigSpec> getGate(std::string gateName);
    std::optional<ConfigSpec> getConfig(std::string configName);
    std::optional<ConfigSpec> getLayer(std::string layerName);
    std::optional<std::string> getExperimentLayer(std::string experimentName);
    void initialize();
    void shutdown();

  private:
    Network *network;
    Options options;
    std::unordered_map<std::string, ConfigSpec> featureGates;
    std::unordered_map<std::string, ConfigSpec> dynamicConfigs;
    std::unordered_map<std::string, ConfigSpec> layerConfigs;
    std::unordered_map<std::string, std::string> experimentToLayer;
    long long lastConfigSyncTime;
    std::optional<boost::thread> syncBgThread;
    void sync();
    void fetchConfigSpecs();
    void processSpecsJSON(nlohmann::json specsJSON);
    std::unordered_map<std::string, std::string>
    reverseLayerToExperimentsMapping(std::unordered_map<std::string, std::vector<std::string>> &map);
  };
}