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
  inline void json_safe_deserialize(const nlohmann::json j, std::string key, T &v)
  {
    if (j.contains(key))
    {
      j.at(key).get_to(v);
    }
  }

  template <typename T>
  inline void type_safe_deserialize(const nlohmann::json j, T &v)
  {
    try
    {
      v = j.get<T>();
    }
    catch (nlohmann::json::type_error &e)
    {
    }
  }

  template <typename T>
  inline void from_json(const nlohmann::json &j, std::optional<T> &v)
  {
    if (j.is_null())
      v = std::nullopt;
    else
      v = j.get<T>();
  }

  // No easy way to support a generic variant template. Manually specify possible variants here:
  inline void from_json(const nlohmann::json &j, std::variant<int, float, double> &v)
  {
    if (int *maybeInt = std::get_if<int>(&v))
    {
      type_safe_deserialize(j, *maybeInt);
    }
    else if (float *maybeFloat = std::get_if<float>(&v))
    {
      type_safe_deserialize(j, *maybeFloat);
    }
    else if (double *maybeDouble = std::get_if<double>(&v))
    {
      type_safe_deserialize(j, *maybeDouble);
    }
  }

  inline void from_json(const nlohmann::json &j, std::variant<std::vector<std::string>, std::vector<statsig::number>, std::string, statsig::number> &v)
  {
    if (std::vector<std::string> *maybeVecString = std::get_if<std::vector<std::string>>(&v))
    {
      type_safe_deserialize(j, *maybeVecString);
    }
    else if (std::vector<statsig::number> *maybeVecNumber = std::get_if<std::vector<statsig::number>>(&v))
    {
      type_safe_deserialize(j, *maybeVecNumber);
    }
    else if (std::string *maybeString = std::get_if<std::string>(&v))
    {
      type_safe_deserialize(j, *maybeString);
    }
    else if (statsig::number *maybeNumber = std::get_if<statsig::number>(&v))
    {
      type_safe_deserialize(j, *maybeNumber);
    }
  }

  inline void from_json(const nlohmann::json &j, std::variant<bool, nlohmann::json> &v)
  {
    if (bool *maybeBool = std::get_if<bool>(&v))
    {
      type_safe_deserialize(j, *maybeBool);
    }
    else if (nlohmann::json *maybeJson = std::get_if<nlohmann::json>(&v))
    {
      type_safe_deserialize(j, *maybeJson);
    }
  }

  inline void from_json(const nlohmann::json &j, statsig::ConfigCondition &v)
  {
    json_safe_deserialize(j, "type", v.type);
    json_safe_deserialize(j, "operator", v.oper);
    json_safe_deserialize(j, "field", v.field);
    json_safe_deserialize(j, "targetValue", v.targetValue);
    json_safe_deserialize(j, "additionalValues", v.additionalValues);
    json_safe_deserialize(j, "idType", v.idType);
    json_safe_deserialize(j, "isDeviceBased", v.isDeviceBased);
  }

  inline void from_json(const nlohmann::json &j, statsig::ConfigRule &v)
  {
    json_safe_deserialize(j, "name", v.name);
    json_safe_deserialize(j, "id", v.id);
    json_safe_deserialize(j, "salt", v.salt);
    json_safe_deserialize(j, "passPercentage", v.passPercentage);
    json_safe_deserialize(j, "conditions", v.conditions);
    json_safe_deserialize(j, "returnValue", v.returnValue);
    json_safe_deserialize(j, "idType", v.idType);
    json_safe_deserialize(j, "configDelegate", v.configDelegate);
    json_safe_deserialize(j, "isExperimentGroup", v.isExperimentGroup);
  }

  inline void from_json(const nlohmann::json &j, statsig::ConfigSpec &v)
  {
    json_safe_deserialize(j, "name", v.name);
    json_safe_deserialize(j, "type", v.type);
    json_safe_deserialize(j, "salt", v.salt);
    json_safe_deserialize(j, "enabled", v.enabled);
    json_safe_deserialize(j, "rules", v.rules);
    json_safe_deserialize(j, "defaultValue", v.defaultValue);
    json_safe_deserialize(j, "idType", v.idType);
    json_safe_deserialize(j, "explicitParameters", v.explicitParameters);
    json_safe_deserialize(j, "entity", v.entity);
    json_safe_deserialize(j, "isActive", v.isActive);
    json_safe_deserialize(j, "hasSharedParams", v.hasSharedParams);
  }

  // Special case where the input json is actually a list of ConfigSpec objects
  // However, we parse it into a map indexed by the name field
  inline void from_json(const nlohmann::json &j, std::unordered_map<std::string, statsig::ConfigSpec> &v)
  {
    try
    {
      std::vector<statsig::ConfigSpec> configSpecs = j.get<std::vector<statsig::ConfigSpec>>();
      for (auto spec : configSpecs)
      {
        v.insert({spec.name, spec});
      }
    }
    catch (...)
    {
    }
  }
}