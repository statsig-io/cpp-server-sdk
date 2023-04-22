#include "statsig.h"
#include "exceptions.h"
#include <iostream>

namespace statsig
{
  void Statsig::initialize(std::string sdkKey)
  {
    if (isInitialized())
    {
      STATSIG_LOG("Statsig already initialized");
      return;
    }
    this->instance = new Server(sdkKey);
  }

  void Statsig::initialize(std::string sdkKey, Options options)
  {
    if (isInitialized())
    {
      STATSIG_LOG("Statsig already initialized");
      return;
    }
    this->instance = new Server(sdkKey, options);
  }

  bool Statsig::checkGate(User user, std::string gate)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    return instance->checkGate(user, gate);
  }

  DynamicConfig Statsig::getConfig(User user, std::string config)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    return instance->getConfig(user, config);
  }

  DynamicConfig Statsig::getExperiment(User user, std::string config)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    return instance->getConfig(user, config);
  }

  Layer Statsig::getLayer(User user, std::string layer)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    return instance->getLayer(user, layer);
  }

  void Statsig::overrideGate(std::string gateName, bool value)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    this->instance->overrideGate(gateName, value);
  }

  void Statsig::overrideGate(std::string gateName, bool value, std::string userID)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    this->instance->overrideGate(gateName, value, userID);
  }

  void Statsig::overrideConfig(std::string configName, std::unordered_map<std::string, JSON::deserializable> value)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    this->instance->overrideConfig(configName, value);
  }

  void Statsig::overrideConfig(std::string configName, std::unordered_map<std::string, JSON::deserializable> value, std::string userID)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    this->instance->overrideConfig(configName, value, userID);
  }

  void Statsig::overrideLayer(std::string layerName, std::unordered_map<std::string, JSON::deserializable> value)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    this->instance->overrideLayer(layerName, value);
  }

  void Statsig::overrideLayer(std::string layerName, std::unordered_map<std::string, JSON::deserializable> value, std::string userID)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    this->instance->overrideLayer(layerName, value, userID);
  }

  void Statsig::removeGateOverride(std::string gateName)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    this->instance->removeGateOverride(gateName);
  }

  void Statsig::removeGateOverride(std::string gateName, std::string userID)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    this->instance->removeGateOverride(gateName, userID);
  }

  void Statsig::removeConfigOverride(std::string configName)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    this->instance->removeConfigOverride(configName);
  }

  void Statsig::removeConfigOverride(std::string configName, std::string userID)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    this->instance->removeConfigOverride(configName, userID);
  }

  void Statsig::removeLayerOverride(std::string layerName)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    this->instance->removeLayerOverride(layerName);
  }

  void Statsig::removeLayerOverride(std::string layerName, std::string userID)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    this->instance->removeLayerOverride(layerName, userID);
  }

  bool Statsig::isInitialized()
  {
    return this->instance;
  }

  void Statsig::shutdown()
  {
    if (!isInitialized())
    {
      STATSIG_LOG("Statsig not initialized");
      return;
    }
    delete this->instance;
  }
}
