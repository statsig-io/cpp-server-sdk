#include "server.h"

namespace statsig
{
  bool Server::checkGate(User user, std::string gateName)
  {
    auto res = this->evaluator->checkGate(user, gateName);
    this->logger->logGateExposure(user, gateName, res, false);
    return res.booleanValue;
  }

  DynamicConfig Server::getConfig(User user, std::string configName)
  {
    auto res = this->evaluator->getConfig(user, configName);
    this->logger->logConfigExposure(user, configName, res, false);
    return DynamicConfig{configName, res.configValue, res.ruleID};
  }

  Layer Server::getLayer(User user, std::string layerName)
  {
    auto res = this->evaluator->getLayer(user, layerName);
    auto logFunction = [this, res, user](Layer layer, std::string parameterName)
    {
      this->logger->logLayerExposure(user, layer, parameterName, res, false);
    };
    return Layer(layerName, res.configValue, res.ruleID, logFunction);
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
    delete this->logger;
    delete this->network;
    delete this->evaluator;
  }
}