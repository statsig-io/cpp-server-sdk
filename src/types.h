#pragma once

#include <any>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <nlohmann/json.hpp>

namespace statsig
{
  using number = std::variant<int, float, double>;

  struct Options
  {
    std::string api;
    Options() : api("https://statsigapi.net/v1"){};
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
    std::unordered_map<std::string, std::any> custom;
    std::unordered_map<std::string, std::any> privateAttribute;
    std::unordered_map<std::string, std::string> statsigEnvironment;
    std::unordered_map<std::string, std::string> customIDs;
    User() : userID(""),
             email(""),
             ipAddress(""),
             userAgent(""),
             country(""),
             locale(""),
             appVersion("")
    {
      custom = std::unordered_map<std::string, std::any>();
      privateAttribute = std::unordered_map<std::string, std::any>();
      statsigEnvironment = std::unordered_map<std::string, std::string>();
      customIDs = std::unordered_map<std::string, std::string>();
    }
  };

  struct DynamicConfig
  {
    std::string name;
    std::unordered_map<std::string, std::any> value;
    std::string ruleID;
    // DynamicConfig() : name(""), value({}), ruleID(""){}
    DynamicConfig()
    {
      name = "";
      value = std::unordered_map<std::string, std::any>();
      ruleID = "";
    };
  };

  struct Event
  {
    std::string eventName;
    User user;
    std::string value;
    std::unordered_map<std::string, std::string> metadata;
    int time;
    // Event() : eventName(""), user(User()), value(""), metadata({}), time(0){};
    Event() : eventName(""), user(User()), value(""), time(0)
    {
      metadata = std::unordered_map<std::string, std::string>();
    };
  };

  struct EvalResult
  {
    bool fetchFromServer;
    bool booleanValue;
    DynamicConfig configValue;
    std::vector<std::unordered_map<std::string, std::string>> secondaryExposures;
    std::vector<std::unordered_map<std::string, std::string>> undelegatedSecondaryExposures;
    std::unordered_map<std::string, bool> explicitParameters;
    bool isExperimentGroup;
    // EvalResult() : fetchFromServer(false),
    //                booleanValue(false),
    //                configValue(DynamicConfig()),
    //                secondaryExposures({}),
    //                undelegatedSecondaryExposures({}),
    //                explicitParameters({}),
    //                isExperimentGroup(false){};
    EvalResult()
    {
      fetchFromServer = false;
      booleanValue = false;
      configValue = DynamicConfig();
      secondaryExposures = std::vector<std::unordered_map<std::string, std::string>>();
      undelegatedSecondaryExposures = std::vector<std::unordered_map<std::string, std::string>>();
      explicitParameters = std::unordered_map<std::string, bool>();
      isExperimentGroup = false;
    };
  };

  struct ConfigCondition
  {
    std::string type;
    std::string idType;
    bool isDeviceBased;
    std::optional<std::string> oper; // operator is a c++ keyword
    std::optional<std::string> field;
    std::optional<std::variant<std::vector<std::string>, std::vector<number>, std::string, number>> targetValue;
    std::optional<std::unordered_map<std::string, std::string>> additionalValues;
    ConfigCondition() : type(""),
                        idType(""),
                        isDeviceBased(false),
                        oper(std::nullopt),
                        field(std::nullopt),
                        targetValue(std::nullopt),
                        additionalValues(std::nullopt){};
  };

  struct ConfigRule
  {
    std::string name;
    std::string id;
    std::string salt;
    double passPercentage;
    std::vector<ConfigCondition> conditions;
    // std::variant<bool, std::unordered_map<std::string, std::any>> returnValue;
    std::variant<bool, nlohmann::json> returnValue;
    std::string idType;
    std::optional<std::string> configDelegate;
    std::optional<bool> isExperimentGroup;
    ConfigRule() : name(""),
                   id(""),
                   salt(""),
                   passPercentage(0.0),
                   conditions(std::vector<ConfigCondition>()),
                   returnValue(false),
                   idType(""),
                   configDelegate(std::nullopt),
                   isExperimentGroup(std::nullopt){};
  };

  struct ConfigSpec
  {
    std::string name;
    std::string type;
    std::string salt;
    bool enabled;
    std::vector<ConfigRule> rules;
    // std::variant<bool, std::unordered_map<std::string, std::any>> defaultValue;
    std::variant<bool, nlohmann::json> defaultValue;
    std::string idType;
    std::optional<std::vector<std::string>> explicitParameters;
    std::optional<std::string> entity;
    std::optional<bool> isActive;
    std::optional<bool> hasSharedParams;
    ConfigSpec() : name(""),
                   type(""),
                   salt(""),
                   enabled(false),
                   rules(std::vector<ConfigRule>()),
                   defaultValue(false),
                   idType(""),
                   explicitParameters(std::nullopt),
                   entity(std::nullopt),
                   isActive(std::nullopt),
                   hasSharedParams(std::nullopt){};
  };

  // NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(
  //     ConfigCondition,
  //     type,
  //     oper,
  //     field,
  //     targetValue,
  //     additionalValues,
  //     idType,
  //     isDeviceBased);
  // NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(
  //     ConfigRule,
  //     name,
  //     id,
  //     salt,
  //     passPercentage,
  //     conditions,
  //     returnValue,
  //     idType,
  //     configDelegate,
  //     isExperimentGroup);
  // NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(
  //     statsig::ConfigSpec,
  //     name,
  //     type,
  //     salt,
  //     enabled,
  //     rules,
  //     defaultValue,
  //     idType,
  //     explicitParameters,
  //     entity,
  //     isActive,
  //     hasSharedParams);
}