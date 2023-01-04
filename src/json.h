#pragma once

#include <nlohmann/json.hpp>

#include "types.h"

namespace JSON
{
  // json::any -- types that are supported by json conversion
  // We cannot use generic std::any because we need to define how to convert each non-STL type
  using any = std::variant<
      bool,
      int,
      float,
      double,
      std::string,
      struct statsig::StatsigMetadata>;
}

namespace nlohmann
{
  // ------------------
  // JSON serialization
  // ------------------
  template <typename T>
  inline void to_json(nlohmann::json &j, const std::optional<T> &v)
  {
    if (v.has_value())
      j = *v;
    else
      j = nullptr;
  }

  inline void to_json(nlohmann::json &j, const struct statsig::StatsigMetadata &v)
  {
    j = json{
        {"sdkType", v.sdkType},
        {"sdkVersion", v.sdkVersion},
    };
  }

  inline void to_json(nlohmann::json &j, const JSON::any &v)
  {
    if (const bool *maybeBool = std::get_if<bool>(&v))
    {
      j = *maybeBool;
    }
    else if (const int *maybeInt = std::get_if<int>(&v))
    {
      j = *maybeInt;
    }
    else if (const float *maybeFloat = std::get_if<float>(&v))
    {
      j = *maybeFloat;
    }
    else if (const double *maybeDouble = std::get_if<double>(&v))
    {
      j = *maybeDouble;
    }
    else if (const std::string *maybeString = std::get_if<std::string>(&v))
    {
      j = *maybeString;
    }
    else if (const struct statsig::StatsigMetadata *maybeStatsigMetadata = std::get_if<struct statsig::StatsigMetadata>(&v))
    {
      j = *maybeStatsigMetadata;
    }
  }

  template <typename K, typename V>
  inline void to_json(nlohmann::json &j, const std::variant<std::map<K, V>, std::unordered_map<K, V>, std::multimap<K, V>> &v)
  {
    if (const std::map<K, V> *maybeMap = std::get_if<std::map<K, V>>(&v))
    {
      j = *maybeMap;
    }
    else if (const std::unordered_map<K, V> *maybeUnorderedMap = std::get_if<std::unordered_map<K, V>>(&v))
    {
      j = *maybeUnorderedMap;
    }
    else if (const std::multimap<K, V> *maybeMultimap = std::get_if<std::multimap<K, V>>(&v))
    {
      j = *maybeMultimap;
    }
  }

  // --------------------
  // JSON deserialization
  // --------------------
  template <typename T>
  inline void from_json(const nlohmann::json &j, std::optional<T> &v)
  {
    if (j.is_null())
      v = std::nullopt;
    else
      v = j.get<T>();
  }

  template <typename T>
  inline void from_json(const nlohmann::json &j, std::vector<T> &v)
  {
    v = j.get<std::vector<T>>();
  }

  // No easy way to support a generic variant template. Manually specify possible variants here:
  inline void from_json(const nlohmann::json &j, std::variant<int, float, double> &v)
  {
    if (std::get_if<int>(&v))
    {
      v = j.get<int>();
    }
    else if (std::get_if<float>(&v))
    {
      v = j.get<float>();
    }
    else if (std::get_if<double>(&v))
    {
      v = j.get<double>();
    }
  }

  inline void from_json(const nlohmann::json &j, std::variant<std::vector<std::string>, std::vector<statsig::number>, std::string, statsig::number> &v)
  {
    if (std::get_if<std::vector<std::string>>(&v))
    {
      v = j.get<std::vector<std::string>>();
    }
    else if (std::get_if<std::vector<statsig::number>>(&v))
    {
      v = j.get<std::vector<statsig::number>>();
    }
    else if (std::get_if<std::string>(&v))
    {
      v = j.get<std::string>();
    }
    else if (std::get_if<statsig::number>(&v))
    {
      v = j.get<statsig::number>();
    }
  }

  inline void from_json(const nlohmann::json &j, std::variant<bool, nlohmann::json> &v)
  {
    if (const bool *maybeBool = std::get_if<bool>(&v))
    {
      v = *maybeBool;
    }
    else if (const nlohmann::json *maybeJson = std::get_if<nlohmann::json>(&v))
    {
      v = *maybeJson;
    }
  }

  inline void from_json(const nlohmann::json &j, statsig::ConfigCondition &v)
  {
    j.at("type").get_to(v.type);
    j.at("operator").get_to(v.oper);
    j.at("field").get_to(v.field);
    j.at("targetValue").get_to(v.targetValue);
    j.at("additionalValues").get_to(v.additionalValues);
    j.at("idType").get_to(v.idType);
    j.at("isDeviceBased").get_to(v.isDeviceBased);
  }

  inline void from_json(const nlohmann::json &j, statsig::ConfigRule &v)
  {
    j.at("name").get_to(v.name);
    j.at("id").get_to(v.id);
    j.at("salt").get_to(v.salt);
    j.at("passPercentage").get_to(v.passPercentage);
    j.at("conditions").get_to(v.conditions);
    j.at("returnValue").get_to(v.returnValue);
    j.at("idType").get_to(v.idType);
    j.at("configDelegate").get_to(v.configDelegate);
    j.at("isExperimentGroup").get_to(v.isExperimentGroup);
  }

  inline void from_json(const nlohmann::json &j, statsig::ConfigSpec &v)
  {
    j.at("name").get_to(v.name);
    j.at("type").get_to(v.type);
    j.at("salt").get_to(v.salt);
    j.at("enabled").get_to(v.enabled);
    j.at("rules").get_to(v.rules);
    j.at("defaultValue").get_to(v.defaultValue);
    j.at("idType").get_to(v.idType);
    j.at("explicitParameters").get_to(v.explicitParameters);
    j.at("entity").get_to(v.entity);
    j.at("isActive").get_to(v.isActive);
    j.at("hasSharedParams").get_to(v.hasSharedParams);
  }

  // Special case where the input json is actually a list of ConfigSpec objects
  // However, we parse it into a map indexed by the name field
  inline void from_json(const nlohmann::json &j, std::unordered_map<std::string, statsig::ConfigSpec> &v)
  {
    try
    {
      auto configSpecs = j.get<std::vector<statsig::ConfigSpec>>();
      for (auto spec : configSpecs)
      {
        v[spec.name] = spec;
      }
    }
    catch (...)
    { // fallback to standard serializer
      v = j.get<std::unordered_map<std::string, statsig::ConfigSpec>>();
    }
  }
}