#include "statsig.h"
#include "exceptions.h"
#include <iostream>

namespace statsig
{
  void Statsig::initialize(std::string sdkKey)
  {
    std::cout << "Initializing" << std::endl;
    if (isInitialized())
    {
      std::cout << "Statsig already initialized" << std::endl;
      return;
    }
    this->client = new Client(sdkKey);
    std::cout << "Initialized" << std::endl;
  }

  void Statsig::initialize(std::string sdkKey, Options options)
  {
    std::cout << "Initializing" << std::endl;
    if (isInitialized())
    {
      std::cout << "Statsig already initialized" << std::endl;
      return;
    }
    this->client = new Client(sdkKey, options);
    std::cout << "Initialized" << std::endl;
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
    return this->client;
  }

  void Statsig::shutdown()
  {
    if (!isInitialized())
    {
      std::cout << "Statsig not initialized" << std::endl;
      return;
    }
    this->client->shutdown();
    this->client = NULL;
  }
}
