#pragma once

#include <nlohmann/json.hpp>

#include "types.h"

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

  inline void to_json(nlohmann::json &j, const JSON::any &v)
  {
    if (const int *maybeInt = std::get_if<int>(&v))
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
    else if (const long *maybeLong = std::get_if<long>(&v))
    {
      j = *maybeLong;
    }
    else if (const long long *maybeLongLong = std::get_if<long long>(&v))
    {
      j = *maybeLongLong;
    }
    else if (const std::string *maybeString = std::get_if<std::string>(&v))
    {
      j = *maybeString;
    }
    else if (const std::vector<bool> *maybeBoolVec = std::get_if<std::vector<bool>>(&v))
    {
      j = *maybeBoolVec;
    }
    else if (const std::vector<int> *maybeIntVec = std::get_if<std::vector<int>>(&v))
    {
      j = *maybeIntVec;
    }
    else if (const std::vector<float> *maybeFloatVec = std::get_if<std::vector<float>>(&v))
    {
      j = *maybeFloatVec;
    }
    else if (const std::vector<double> *maybeDoubleVec = std::get_if<std::vector<double>>(&v))
    {
      j = *maybeDoubleVec;
    }
    else if (const std::vector<long> *maybeLongVec = std::get_if<std::vector<long>>(&v))
    {
      j = *maybeLongVec;
    }
    else if (const std::vector<long long> *maybeLongLongVec = std::get_if<std::vector<long long>>(&v))
    {
      j = *maybeLongLongVec;
    }
    else if (const std::vector<std::string> *maybeStringVec = std::get_if<std::vector<std::string>>(&v))
    {
      j = *maybeStringVec;
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

  inline void to_json(nlohmann::json &j, const std::variant<bool, std::unordered_map<std::string, JSON::any>> &v)
  {
    if (const bool *maybeBool = std::get_if<bool>(&v))
    {
      j = *maybeBool;
    }
    else if (const std::unordered_map<std::string, JSON::any> *maybeMap = std::get_if<std::unordered_map<std::string, JSON::any>>(&v))
    {
      j = *maybeMap;
    }
  }

  inline void to_json(nlohmann::json &j, const struct statsig::StatsigMetadata &v)
  {
    j = json{
        {"sdkType", v.sdkType},
        {"sdkVersion", v.sdkVersion},
    };
  }

  inline void to_json(nlohmann::json &j, const statsig::DynamicConfig &v)
  {
    j = json{
        {"name", v.name},
        {"value", v.value},
        {"ruleID", v.ruleID},
    };
  }

  inline void to_json(nlohmann::json &j, const statsig::EvalResult &v)
  {
    j = json{
        {"fetchFromServer", v.fetchFromServer},
        {"booleanValue", v.booleanValue},
        {"configValue", v.configValue},
        {"ruleID", v.ruleID},
        {"secondaryExposures", v.secondaryExposures},
        {"undelegatedSecondaryExposures", v.undelegatedSecondaryExposures},
        {"explicitParameters", v.explicitParameters},
        {"isExperimentGroup", v.isExperimentGroup},
    };
  }

  inline void to_json(nlohmann::json &j, const statsig::ConfigCondition &v)
  {
    j = json{
        {"type", v.type},
        {"operator", v.op},
        {"field", v.field},
        {"targetValue", v.targetValue},
        {"additionalValues", v.additionalValues},
        {"idType", v.idType},
        {"isDeviceBased", v.isDeviceBased},
    };
  }

  inline void to_json(nlohmann::json &j, const statsig::ConfigRule &v)
  {
    j = json{
        {"name", v.name},
        {"id", v.id},
        {"salt", v.salt},
        {"passPercentage", v.passPercentage},
        {"conditions", v.conditions},
        {"returnValue", v.returnValue},
        {"idType", v.idType},
        {"configDelegate", v.configDelegate},
        {"isExperimentGroup", v.isExperimentGroup},
    };
  }

  inline void to_json(nlohmann::json &j, const statsig::ConfigSpec &v)
  {
    j = json{
        {"name", v.name},
        {"type", v.type},
        {"salt", v.salt},
        {"enabled", v.enabled},
        {"rules", v.rules},
        {"defaultValue", v.defaultValue},
        {"idType", v.idType},
        {"explicitParameters", v.explicitParameters},
        {"entity", v.entity},
        {"isActive", v.isActive},
        {"hasSharedParams", v.hasSharedParams},
    };
  }

  // --------------------
  // JSON deserialization
  // --------------------
  template <typename T>
  inline void json_safe_deserialize(const nlohmann::json j, std::string key, T &v, bool isOptional = false)
  {
    if (!isOptional || j.contains(key))
    {
      j.at(key).get_to(v);
    }
  }

  template <typename T, typename... Ts>
  inline void type_safe_deserialize(const nlohmann::json j, std::variant<Ts...> &v)
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
  inline void from_json(const nlohmann::json &j, std::variant<int, float, double, long, long long> &v)
  {
    type_safe_deserialize<int>(j, v);
    type_safe_deserialize<float>(j, v);
    type_safe_deserialize<double>(j, v);
    type_safe_deserialize<long>(j, v);
    type_safe_deserialize<long long>(j, v);
  }

  inline void from_json(const nlohmann::json &j, std::variant<std::vector<std::string>, std::vector<statsig::number>, std::string, statsig::number> &v)
  {
    type_safe_deserialize<std::vector<std::string>>(j, v);
    type_safe_deserialize<std::vector<statsig::number>>(j, v);
    type_safe_deserialize<std::string>(j, v);
    type_safe_deserialize<statsig::number>(j, v);
  }

  inline void from_json(const nlohmann::json &j, std::variant<bool, std::unordered_map<std::string, JSON::any>> &v)
  {
    type_safe_deserialize<bool>(j, v);
    type_safe_deserialize<std::unordered_map<std::string, JSON::any>>(j, v);
  }

  inline void from_json(const nlohmann::json &j, JSON::any &v)
  {
    type_safe_deserialize<int>(j, v);
    type_safe_deserialize<float>(j, v);
    type_safe_deserialize<double>(j, v);
    type_safe_deserialize<long>(j, v);
    type_safe_deserialize<long long>(j, v);
    type_safe_deserialize<std::string>(j, v);
    type_safe_deserialize<std::vector<bool>>(j, v);
    type_safe_deserialize<std::vector<int>>(j, v);
    type_safe_deserialize<std::vector<float>>(j, v);
    type_safe_deserialize<std::vector<double>>(j, v);
    type_safe_deserialize<std::vector<long>>(j, v);
    type_safe_deserialize<std::vector<long long>>(j, v);
    type_safe_deserialize<std::vector<std::string>>(j, v);
    type_safe_deserialize<struct statsig::StatsigMetadata>(j, v);
  }

  inline void from_json(const nlohmann::json &j, struct statsig::StatsigMetadata &v)
  {
    json_safe_deserialize(j, "sdkType", v.sdkType);
    json_safe_deserialize(j, "sdkVersion", v.sdkVersion);
  }

  inline void from_json(const nlohmann::json &j, statsig::DynamicConfig &v)
  {
    json_safe_deserialize(j, "name", v.name);
    json_safe_deserialize(j, "value", v.value);
    json_safe_deserialize(j, "ruleID", v.ruleID);
  }

  inline void from_json(const nlohmann::json &j, statsig::ConfigCondition &v)
  {
    json_safe_deserialize(j, "type", v.type);
    json_safe_deserialize(j, "operator", v.op, true);
    json_safe_deserialize(j, "field", v.field, true);
    json_safe_deserialize(j, "targetValue", v.targetValue, true);
    json_safe_deserialize(j, "additionalValues", v.additionalValues, true);
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
    json_safe_deserialize(j, "configDelegate", v.configDelegate, true);
    json_safe_deserialize(j, "isExperimentGroup", v.isExperimentGroup, true);
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
    json_safe_deserialize(j, "explicitParameters", v.explicitParameters, true);
    json_safe_deserialize(j, "entity", v.entity, true);
    json_safe_deserialize(j, "isActive", v.isActive, true);
    json_safe_deserialize(j, "hasSharedParams", v.hasSharedParams, true);
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