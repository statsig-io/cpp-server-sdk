#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <nlohmann/json.hpp>

#include "statsig_metadata.h"

namespace JSON
{
  // json::any -- types that are supported by json conversion
  // We cannot use generic std::any because we need to define how to convert each non-STL type
  using any = std::variant<
      int,
      float,
      double,
      long,
      long long,
      std::string,
      std::vector<bool>,
      std::vector<int>,
      std::vector<float>,
      std::vector<double>,
      std::vector<long>,
      std::vector<long long>,
      std::vector<std::string>,
      struct statsig::StatsigMetadata>;
}

namespace statsig
{
  using number = std::variant<int, float, double, long, long long>;

  struct Options
  {
    std::string api;
    Options() : api("https://statsigapi.net"){};
  };

  struct User
  {
    std::string userID;
    std::string email;
    std::string ipAddress;
    std::string userAgent;
    std::string country;
    std::string locale;
    std::string appVersion;
    std::unordered_map<std::string, JSON::any> custom;
    std::unordered_map<std::string, JSON::any> privateAttribute;
    std::unordered_map<std::string, std::string> statsigEnvironment;
    std::unordered_map<std::string, std::string> customIDs;
    User() : userID(""),
             email(""),
             ipAddress(""),
             userAgent(""),
             country(""),
             locale(""),
             appVersion(""),
             custom(std::unordered_map<std::string, JSON::any>()),
             privateAttribute(std::unordered_map<std::string, JSON::any>()),
             statsigEnvironment(std::unordered_map<std::string, std::string>()),
             customIDs(std::unordered_map<std::string, std::string>()) {}
  };

  struct DynamicConfig
  {
    std::string name;
    std::unordered_map<std::string, JSON::any> value;
    std::string ruleID;
  };

  struct Event
  {
    std::string eventName;
    User user;
    std::string value;
    std::unordered_map<std::string, std::string> metadata;
    int time;
  };

  struct EvalResult
  {
    bool fetchFromServer;
    bool booleanValue;
    std::unordered_map<std::string, JSON::any> configValue;
    std::string ruleID;
    std::vector<std::unordered_map<std::string, std::string>> secondaryExposures;
    std::vector<std::unordered_map<std::string, std::string>> undelegatedSecondaryExposures;
    std::optional<std::vector<std::string>> explicitParameters;
    bool isExperimentGroup;
  };

  struct ConfigCondition
  {
    std::string type;
    std::string idType;
    bool isDeviceBased;
    std::optional<std::string> op; // operator is a c++ keyword
    std::optional<std::string> field;
    std::optional<JSON::any> targetValue;
    std::optional<std::unordered_map<std::string, std::string>> additionalValues;
  };

  struct ConfigRule
  {
    std::string name;
    std::string id;
    std::string salt;
    double passPercentage;
    std::vector<ConfigCondition> conditions;
    std::variant<bool, std::unordered_map<std::string, JSON::any>> returnValue;
    std::string idType;
    std::optional<std::string> configDelegate;
    std::optional<bool> isExperimentGroup;
  };

  struct ConfigSpec
  {
    std::string name;
    std::string type;
    std::string salt;
    bool enabled;
    bool isDeviceBased;
    std::vector<ConfigRule> rules;
    std::variant<bool, std::unordered_map<std::string, JSON::any>> defaultValue;
    std::string idType;
    std::optional<std::vector<std::string>> explicitParameters;
    std::optional<std::string> entity;
    std::optional<bool> isActive;
    std::optional<bool> hasSharedParams;
  };
}