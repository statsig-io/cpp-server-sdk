#pragma once

#include "network.h"
#include "store.h"
#include "types.h"

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
  };
}