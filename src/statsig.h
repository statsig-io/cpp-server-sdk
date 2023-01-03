#pragma once

#include "client.h"
#include "evaluator.h"
#include "types.h"
#include <nlohmann/json.hpp>
#include <httplib.h>

namespace statsig
{
  class Statsig
  {
  public:
    void initialize(std::string sdkKey);
    void initialize(std::string sdkKey, Options options);
    bool checkGate(User user, std::string gate);
    DynamicConfig getConfig(User user, std::string config);
    void shutdown();
    bool isInitialized();

  private:
    Client *client;
  };
}