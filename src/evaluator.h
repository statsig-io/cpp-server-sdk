#pragma once

#include <boost/algorithm/string.hpp>

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
    void shutdown();

  private:
    Network *network;
    Options options;
    Store *store;
    EvalResult evaluate(User user, ConfigSpec spec);
    EvalResult evaluateRule(User user, ConfigRule rule);
    EvalResult evaluateCondition(User user, ConfigCondition condition);
    std::optional<EvalResult> evaluateDelegate(User user, ConfigRule rule, std::vector<std::unordered_map<std::string, std::string>> exposures);
    bool evalPassPercentage(User user, ConfigRule rule, ConfigSpec spec);
    std::optional<JSON::any> getFromUser(User user, std::string field);
    std::optional<JSON::any> getFromUserAgent(User user, std::string field);
    std::optional<JSON::any> getFromEnvironment(User user, std::string field);
    std::string getUnitID(User user, std::string idType);
  };
}