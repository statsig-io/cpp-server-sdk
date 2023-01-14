#pragma once

#include "evaluator_utils.h"
#include "network.h"
#include "store.h"
#include "types.h"
#include "utils.h"

namespace statsig
{
  class Evaluator
  {
  public:
    Evaluator(Network *network, Options options)
    {
      this->network = network;
      this->options = options;
      this->store = new Store(network, options);
      this->store->initialize();
    };
    EvalResult checkGate(User user, std::string gate);
    EvalResult getConfig(User user, std::string config);
    void overrideGate(std::string gateName, bool value,
                      std::optional<std::string> userID = std::nullopt);
    void overrideConfig(std::string configName,
                        std::unordered_map<std::string, JSON::deserializable> value,
                        std::optional<std::string> userID = std::nullopt);
    void overrideLayer(std::string layerName,
                       std::unordered_map<std::string, JSON::deserializable> value,
                       std::optional<std::string> userID = std::nullopt);
    void removeGateOverride(std::string gateName, std::optional<std::string> userID = std::nullopt);
    void removeConfigOverride(std::string configName, std::optional<std::string> userID = std::nullopt);
    void removeLayerOverride(std::string layerName, std::optional<std::string> userID = std::nullopt);
    void shutdown();

  private:
    Network *network;
    Options options;
    Store *store;
    struct overrides
    {
      std::unordered_map<std::string, std::unordered_map<std::string, bool>> gates;
      std::unordered_map<
          std::string,
          std::unordered_map<std::string, std::unordered_map<std::string, JSON::deserializable>>>
          configs;
      std::unordered_map<
          std::string,
          std::unordered_map<std::string, std::unordered_map<std::string, JSON::deserializable>>>
          layers;
    } overrides;
    EvalResult evaluate(User user, ConfigSpec spec);
    EvalResult evaluateRule(User user, ConfigRule rule);
    EvalResult evaluateCondition(User user, ConfigCondition condition);
    std::optional<EvalResult>
    evaluateDelegate(User user, ConfigRule rule,
                     std::vector<std::unordered_map<std::string, std::string>> exposures);
    bool evalPassPercentage(User user, ConfigRule rule, ConfigSpec spec);
    std::optional<JSON::any> getFromUser(User user, std::string field);
    std::optional<JSON::any> getFromUserAgent(User user, std::string field);
    std::optional<JSON::any> getFromEnvironment(User user, std::string field);
    std::string getUnitID(User user, std::string idType);
    std::optional<EvalResult> checkForGateOverrides(User user, std::string gateName);
    std::optional<EvalResult> checkForConfigOverrides(User user, std::string configName);
    std::optional<EvalResult> checkForLayerOverrides(User user, std::string layerName);
  };
}