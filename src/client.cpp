#include "client.h"

namespace statsig
{
  bool Client::checkGate(User user, std::string gate)
  {
    auto res = this->evaluator->checkGate(user, gate);
    return res.booleanValue;
  }

  DynamicConfig Client::getConfig(User user, std::string config)
  {
    auto res = this->evaluator->getConfig(user, config);
    return DynamicConfig{config, res.configValue, res.ruleID};
  }

  void Client::shutdown()
  {
    this->network->shutdown();
    this->evaluator->shutdown();
    this->network = NULL;
    this->evaluator = NULL;
  }
}