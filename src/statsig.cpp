#include "statsig.h"
#include "exceptions.h"

namespace statsig
{
  void Statsig::initialize(std::string sdkKey)
  {
    if (isInitialized())
    {
      return;
    }
    client = new Client(sdkKey);
  }

  void Statsig::initialize(std::string sdkKey, Options options)
  {
    if (isInitialized())
    {
      return;
    }
    client = new Client(sdkKey, options);
  }

  bool Statsig::checkGate(User user, std::string gate)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    return client->checkGate(user, gate);
  }

  DynamicConfig Statsig::getConfig(User user, std::string config)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    return client->getConfig(user, config);
  }

  bool Statsig::isInitialized()
  {
    return client && client->initialized;
  }

  void Statsig::shutdown()
  {
    if (!isInitialized())
    {
      return;
    }
    client->shutdown();
    client = NULL;
  }
}
