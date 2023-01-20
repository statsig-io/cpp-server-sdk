#pragma once

#include "evaluator.h"
#include "event_logger.h"
#include "types.h"
#include "network.h"
#include "layer.h"

namespace statsig
{
  class Server
  {
  public:
    Server(std::string sdkKey, Options options = Options())
    {
      this->sdkKey = sdkKey;
      this->options = options;
      this->network = new Network(sdkKey, options);
      this->evaluator = new Evaluator(network, options);
      this->logger = new EventLogger(network, options);
    };
    ~Server() { shutdown(); };
    bool checkGate(User user, std::string gateName);
    DynamicConfig getConfig(User user, std::string configName);
    Layer getLayer(User user, std::string layerName);
    void overrideGate(std::string gateName, bool value, std::optional<std::string> userID = std::nullopt);
    void overrideConfig(std::string configName, std::unordered_map<std::string, JSON::deserializable> value, std::optional<std::string> userID = std::nullopt);
    void overrideLayer(std::string layerName, std::unordered_map<std::string, JSON::deserializable> value, std::optional<std::string> userID = std::nullopt);
    void removeGateOverride(std::string gateName, std::optional<std::string> userID = std::nullopt);
    void removeConfigOverride(std::string configName, std::optional<std::string> userID = std::nullopt);
    void removeLayerOverride(std::string layerName, std::optional<std::string> userID = std::nullopt);
    void shutdown();

  private:
    std::string sdkKey;
    Options options;
    Network *network;
    Evaluator *evaluator;
    EventLogger *logger;
  };
}