#pragma once

#include "evaluator.h"
#include "types.h"
#include "network.h"

namespace statsig
{
  class Client
  {
  public:
    Client(std::string sdkKey, Options options = Options())
    {
      this->sdkKey = sdkKey;
      this->options = options;
      this->network = new Network(sdkKey, options);
      this->evaluator = new Evaluator(network, options);
    }
    bool checkGate(User user, std::string gate);
    DynamicConfig getConfig(User user, std::string config);
    void shutdown();

  private:
    std::string sdkKey;
    Options options;
    Network *network;
    Evaluator *evaluator;
  };
}