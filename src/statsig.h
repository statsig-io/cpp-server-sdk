#pragma once

#include <iostream>

#include <nlohmann/json.hpp>
#include <httplib.h>

#include "evaluator.h"
#include "exceptions.h"
#include "layer.h"
#include "output_logger.h"
#include "server.h"
#include "types.h"

namespace statsig
{
  extern Server *instance;

  static bool isInitialized()
  {
    return instance != nullptr;
  }

  static void initialize(std::string sdkKey)
  {
    if (isInitialized())
    {
      STATSIG_ERROR("Statsig already initialized");
      return;
    }
    instance = new Server(sdkKey);
    STATSIG_LOG("Statsig initialized");
  }

  static void initialize(std::string sdkKey, Options options)
  {
    if (isInitialized())
    {
      STATSIG_ERROR("Statsig already initialized");
      return;
    }
    instance = new Server(sdkKey, options);
    STATSIG_LOG("Statsig initialized");
  }

  static bool checkGate(User user, std::string gate)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    return instance->checkGate(user, gate);
  }

  static DynamicConfig getConfig(User user, std::string config)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    return instance->getConfig(user, config);
  }

  static DynamicConfig getExperiment(User user, std::string config)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    return instance->getConfig(user, config);
  }

  static Layer getLayer(User user, std::string layer)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    return instance->getLayer(user, layer);
  }

  static void logEvent(User user, std::string eventName)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    return instance->logEvent(user, eventName);
  }

  static void overrideGate(std::string gateName, bool value)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    instance->overrideGate(gateName, value);
  }

  static void overrideGate(std::string gateName, bool value, std::string userID)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    instance->overrideGate(gateName, value, userID);
  }

  static void overrideConfig(std::string configName, std::unordered_map<std::string, JSON::deserializable> value)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    instance->overrideConfig(configName, value);
  }

  static void overrideConfig(std::string configName, std::unordered_map<std::string, JSON::deserializable> value, std::string userID)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    instance->overrideConfig(configName, value, userID);
  }

  static void overrideLayer(std::string layerName, std::unordered_map<std::string, JSON::deserializable> value)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    instance->overrideLayer(layerName, value);
  }

  static void overrideLayer(std::string layerName, std::unordered_map<std::string, JSON::deserializable> value, std::string userID)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    instance->overrideLayer(layerName, value, userID);
  }

  static void removeGateOverride(std::string gateName)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    instance->removeGateOverride(gateName);
  }

  static void removeGateOverride(std::string gateName, std::string userID)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    instance->removeGateOverride(gateName, userID);
  }

  static void removeConfigOverride(std::string configName)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    instance->removeConfigOverride(configName);
  }

  static void removeConfigOverride(std::string configName, std::string userID)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    instance->removeConfigOverride(configName, userID);
  }

  static void removeLayerOverride(std::string layerName)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    instance->removeLayerOverride(layerName);
  }

  static void removeLayerOverride(std::string layerName, std::string userID)
  {
    if (!isInitialized())
    {
      throw StatsigNotInitializedError();
    }
    instance->removeLayerOverride(layerName, userID);
  }

  static void shutdown()
  {
    if (!isInitialized())
    {
      STATSIG_ERROR("Statsig not initialized");
      return;
    }
    delete instance;
    instance = nullptr;
    STATSIG_LOG("Statsig shutdown");
  }
}