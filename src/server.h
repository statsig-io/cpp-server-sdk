#pragma once

#include "evaluator.h"
#include "event_logger.h"
#include "types.h"
#include "network.h"

namespace statsig
{
  class Server
  {
  public:
    Server(std::string sdkKey, Options options = Options())
    {
      this->sdkKey = sdkKey;
      this->options = options;
      this->network = new Network(sdkKey, options);
      this->evaluator = new Evaluator(network, options);
      this->logger = new EventLogger(network, options);
    }
    bool checkGate(User user, std::string gate);
    DynamicConfig getConfig(User user, std::string config);
    void shutdown();

  private:
    std::string sdkKey;
    Options options;
    Network *network;
    Evaluator *evaluator;
    EventLogger *logger;
  };
}