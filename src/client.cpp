#include "client.h"

namespace statsig
{
  bool Client::checkGate(User user, std::string gate)
  {
    auto res = this->evaluator->checkGate(user, gate);
    this->logger->logGateExposure(user, gate, res, false);
    return res.booleanValue;
  }

  DynamicConfig Client::getConfig(User user, std::string config)
  {
    auto res = this->evaluator->getConfig(user, config);
    this->logger->logConfigExposure(user, config, res, false);
    return DynamicConfig{config, res.configValue, res.ruleID};
  }

  void Client::shutdown()
  {
    this->logger->shutdown();
    this->network->shutdown();
    this->evaluator->shutdown();
    this->logger = NULL;
    this->network = NULL;
    this->evaluator = NULL;
  }
}