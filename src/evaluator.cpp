#include "evaluator.h"

namespace statsig
{
  EvalResult Evaluator::checkGate(User user, std::string gate)
  {
    auto maybeGateSpec = this->store->getGate(gate);
    auto gateSpec = maybeGateSpec.value_or(ConfigSpec());
    return EvalResult();
    // if (res.fetchFromServer)
    // return false;
  }

  EvalResult Evaluator::getConfig(User user, std::string config)
  {
    auto maybeConfigSpec = this->store->getConfig(config);
    auto configSpec = maybeConfigSpec.value_or(ConfigSpec());
    return EvalResult();
    // return DynamicConfig();
  }

  void Evaluator::shutdown()
  {
    this->store->shutdown();
    this->store = NULL;
  }
}
