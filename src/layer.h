#pragma once

#include <any>

#include "types.h"
#include "util.h"

namespace statsig
{
  class Layer
  {
  public:
    Layer(
        std::string name,
        std::unordered_map<std::string, JSON::any> value,
        std::string ruleID,
        std::function<void(Layer, std::string)> logExposure)
    {
      this->name = name;
      this->value = value;
      this->ruleID = ruleID;
      this->logExposure = logExposure;
    }
    std::string name;
    std::unordered_map<std::string, JSON::any> value;
    std::string ruleID;
    std::function<void(Layer, std::string)> logExposure;
    template <typename T>
    T get(std::string key, T fallback)
    {
      std::optional<JSON::any> result = Utils::safeGetMap(this->value, key);
      if (!result)
      {
        return fallback;
      }
      if (const auto *maybeResult = std::get_if<T>(&result.value()))
      {
        this->logExposure(*this, key);
        return *maybeResult;
      }
      return fallback;
    };
    template <typename T>
    std::optional<T> get(std::string key)
    {
      std::optional<JSON::any> result = Utils::safeGetMap(this->value, key);
      if (!result)
      {
        return std::nullopt;
      }
      if (const auto *maybeResult = std::get_if<T>(&result.value()))
      {
        this->logExposure(*this, key);
        return *maybeResult;
      }
      return std::nullopt;
    };
  };
}