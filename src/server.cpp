#include "server.h"

namespace statsig
{
  bool Server::checkGate(User user, std::string gate)
  {
    auto res = this->evaluator->checkGate(user, gate);
    this->logger->logGateExposure(user, gate, res, false);
    return res.booleanValue;
  }

  DynamicConfig Server::getConfig(User user, std::string config)
  {
    auto res = this->evaluator->getConfig(user, config);
    this->logger->logConfigExposure(user, config, res, false);
    return DynamicConfig{config, res.configValue, res.ruleID};
  }

  void Server::overrideGate(std::string gateName, bool value, std::optional<std::string> userID)
  {
    this->evaluator->overrideGate(gateName, value, userID);
  }

  void Server::overrideConfig(std::string configName, std::unordered_map<std::string, JSON::deserializable> value, std::optional<std::string> userID)
  {
    this->evaluator->overrideConfig(configName, value, userID);
  }

  void Server::overrideLayer(std::string layerName, std::unordered_map<std::string, JSON::deserializable> value, std::optional<std::string> userID)
  {
    this->evaluator->overrideLayer(layerName, value, userID);
  }

  void Server::removeGateOverride(std::string gateName, std::optional<std::string> userID)
  {
    this->evaluator->removeGateOverride(gateName, userID);
  }

  void Server::removeConfigOverride(std::string configName, std::optional<std::string> userID)
  {
    this->evaluator->removeConfigOverride(configName, userID);
  }

  void Server::removeLayerOverride(std::string layerName, std::optional<std::string> userID)
  {
    this->evaluator->removeLayerOverride(layerName, userID);
  }

  void Server::shutdown()
  {
    this->logger->shutdown();
    this->network->shutdown();
    this->evaluator->shutdown();
    this->logger = NULL;
    this->network = NULL;
    this->evaluator = NULL;
  }
}