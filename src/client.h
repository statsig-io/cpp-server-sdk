#pragma once

#include "evaluator.h"
#include "types.h"
#include "network.h"
#include "store.h"

namespace statsig
{
  class Client
  {
  public:
    Client(std::string sdkKey, Options options = Options())
    {
      sdkKey = sdkKey;
      options = options;
      evaluator = new Evaluator();
      network = new Network(sdkKey, options);
      store = new Store(network, options);
    }
    bool checkGate(User user, std::string gate);
    DynamicConfig getConfig(User user, std::string config);
    void shutdown();
    bool initialized;

  private:
    std::string sdkKey;
    Options options;
    Evaluator *evaluator;
    Store *store;
    Network *network;
  };
}