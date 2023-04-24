#pragma once

#include <iostream>

#include <nlohmann/json.hpp>
#include <httplib.h>

#include "evaluator.h"
#include "layer.h"
#include "output_logger.h"
#include "server.h"
#include "types.h"

namespace statsig
{
  class Statsig
  {
  public:
    Statsig() : instance(NULL){};
    // ~Statsig() { shutdown(); };
    void initialize(std::string sdkKey);
    void initialize(std::string sdkKey, Options options);
    bool checkGate(User user, std::string gate);
    DynamicConfig getConfig(User user, std::string config);
    Layer getLayer(User user, std::string layer);
    void logEvent(User user, std::string eventName);
    void overrideGate(std::string gateName, bool value);
    void overrideGate(std::string gateName, bool value, std::string userID);
    void overrideConfig(std::string configName, std::unordered_map<std::string, JSON::deserializable> value);
    void overrideConfig(std::string configName, std::unordered_map<std::string, JSON::deserializable> value, std::string userID);
    void overrideLayer(std::string layerName, std::unordered_map<std::string, JSON::deserializable> value);
    void overrideLayer(std::string layerName, std::unordered_map<std::string, JSON::deserializable> value, std::string userID);
    void removeGateOverride(std::string gateName);
    void removeGateOverride(std::string gateName, std::string userID);
    void removeConfigOverride(std::string configName);
    void removeConfigOverride(std::string configName, std::string userID);
    void removeLayerOverride(std::string layerName);
    void removeLayerOverride(std::string layerName, std::string userID);
    void shutdown();
    bool isInitialized();

  private:
    Server *instance;
  };
}