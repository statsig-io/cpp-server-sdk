#include "evaluator.h"

namespace statsig
{
  EvalResult Evaluator::checkGate(User user, std::string gate)
  {
    auto maybeSpec = this->store->getGate(gate);
    if (!maybeSpec)
    {
      return EvalResult();
    }
    auto spec = maybeSpec.value_or(ConfigSpec());
    return evaluate(user, spec);
    return EvalResult();
  }

  EvalResult Evaluator::getConfig(User user, std::string config)
  {
    auto maybeSpec = this->store->getConfig(config);
    if (!maybeSpec)
    {
      return EvalResult();
    }
    auto spec = maybeSpec.value_or(ConfigSpec());
    return evaluate(user, spec);
  }

  EvalResult Evaluator::evaluate(User user, ConfigSpec spec)
  {
    std::vector<std::unordered_map<std::string, std::string>> exposures;
    std::unordered_map<std::string, JSON::any> defaultValue;
    std::optional<std::unordered_map<std::string, JSON::any>> defaultJson = Utils::typeSafeGet<std::unordered_map<std::string, JSON::any>>(spec.defaultValue);
    if (defaultJson)
    {
      nlohmann::json j = defaultJson.value();
      json_safe_deserialize(j, defaultValue);
    }
    if (!spec.enabled)
    {
      return EvalResult();
    }
    for (const auto rule : spec.rules)
    {
      auto result = evaluateRule(user, rule);
      if (result.fetchFromServer)
      {
        return result;
      }
      if (result.secondaryExposures.size() > 0)
      {
        exposures.insert(exposures.end(), result.secondaryExposures.begin(),
                         result.secondaryExposures.end());
      }
      if (result.booleanValue)
      {
        auto delegatedResult = evaluateDelegate(user, rule, exposures);
        if (delegatedResult)
        {
          return delegatedResult.value();
        }
        bool pass = evalPassPercentage(user, rule, spec);
        std::unordered_map<std::string, JSON::any> configValue = pass ? result.configValue : defaultValue;
        return EvalResult{
            false, pass, configValue, result.ruleID, exposures, exposures, spec.explicitParameters,
            false};
      }
    }
    return EvalResult{false, false, defaultValue, "default",
                      exposures, exposures, std::nullopt, false};
  }

  EvalResult Evaluator::evaluateRule(User user, ConfigRule rule)
  {
    std::vector<std::unordered_map<std::string, std::string>> exposures;
    bool pass = true;
    for (const auto condition : rule.conditions)
    {
      auto result = evaluateCondition(user, condition);
      if (result.fetchFromServer)
      {
        return result;
      }
      if (result.secondaryExposures.size() > 0)
      {
        exposures.insert(exposures.end(), result.secondaryExposures.begin(),
                         result.secondaryExposures.end());
      }
      if (!result.booleanValue)
      {
        pass = false;
      }
    }
    std::unordered_map<std::string, JSON::any> returnValue;
    std::optional<std::unordered_map<std::string, JSON::any>> defaultJson = Utils::typeSafeGet<std::unordered_map<std::string, JSON::any>>(rule.returnValue);
    if (defaultJson)
    {
      nlohmann::json j = defaultJson.value();
      json_safe_deserialize(j, returnValue);
    }
    return EvalResult{false, pass, returnValue, rule.id,
                      exposures, exposures, std::nullopt, rule.isExperimentGroup.value_or(false)};
  }

  EvalResult Evaluator::evaluateCondition(User user, ConfigCondition condition)
  {
    std::optional<JSON::any> value;
    std::string type = boost::algorithm::to_upper_copy(condition.type);
    std::string field = condition.field.value_or("");
    std::string idType = condition.idType;
    auto target = condition.targetValue;
    std::optional<int> targetAstInt = Utils::typeSafeGet<int>(condition.targetValue).value_or(0);
    if (type == "PUBLIC")
    {
      return EvalResult{false, true};
    }
    else if (type == "FAIL_GATE" || type == "PASS_GATE")
    {
      std::string targetAsString = Utils::typeSafeGet<std::string>(target).value_or("");
      auto gateResult = checkGate(user, targetAsString);
      if (gateResult.fetchFromServer)
      {
        return EvalResult{true};
      }
      std::vector<std::unordered_map<std::string, std::string>> exposures;
      exposures.push_back({{"gate", targetAsString},
                           {"gateValue", gateResult.booleanValue ? "true" : "false"},
                           {"ruleID", gateResult.ruleID}});
      if (gateResult.secondaryExposures.size() > 0)
      {
        exposures.insert(exposures.end(), gateResult.secondaryExposures.begin(),
                         gateResult.secondaryExposures.end());
      }
      bool pass = type == "PASS_GATE" ? gateResult.booleanValue : !gateResult.booleanValue;
      return EvalResult{gateResult.fetchFromServer, pass, std::unordered_map<std::string, JSON::any>(), "", exposures, exposures};
    }
    else if (type == "IP_BASED")
    {
      value = getFromUser(user, field);
      if (!value)
      {
        auto ip = getFromUser(user, "ip");
        if (ip && field == "country")
        {
          // TODO: country lookup
        }
      }
      if (!value)
      {
        return EvalResult{false, false};
      }
    }
    else if (type == "UA_BASED")
    {
      value = getFromUserAgent(user, field);
    }
    else if (type == "USER_FIELD")
    {
      value = getFromUser(user, field);
    }
    else if (type == "CURRENT_TIME")
    {
      value = Utils::getCurrentTimeMS();
    }
    else if (type == "ENVIRONMENT_FIELD")
    {
      value = getFromEnvironment(user, field);
    }
    else if (type == "USER_BUCKET")
    {
      value = getFromEnvironment(user, field);
    }
    else if (type == "UNIT_ID")
    {
      value = getUnitID(user, idType);
    }
    else
    {
      return EvalResult{true};
    }

    std::optional<double> valueAsDouble = Utils::typeSafeGet<double>(value);
    std::optional<double> targetAsDouble = Utils::typeSafeGet<double>(target);
    std::optional<std::string> valueAsString = Utils::typeSafeGet<std::string>(value);
    std::optional<std::string> targetAsString = Utils::typeSafeGet<std::string>(target);
    std::optional<std::vector<std::string>> targetAsVector =
        Utils::typeSafeGet<std::vector<std::string>>(target);
    auto valueAsTime = EvaluatorUtils::getPosixTime(value.value_or(0));
    auto targetAsTime = EvaluatorUtils::getPosixTime(target.value_or(0));

    std::string op = condition.op.value_or("");
    if (op == "gt" || op == "gte" || op == "lt" || op == "lte")
    {
      if (!valueAsDouble || !targetAsDouble)
      {
        return EvalResult{false, false};
      }
      std::function<int(double, double)> compareFn = [](double a, double b) -> int
      { return a - b; };
      return EvalResult{false, EvaluatorUtils::compare(valueAsDouble.value(), targetAsDouble.value(), op, compareFn)};
    }
    else if (op == "version_gt" || op == "version_gte" || op == "version_lt" ||
             op == "version_lte" || op == "version_eq" || op == "version_neq")
    {
      if (!valueAsString || !targetAsString)
      {
        return EvalResult{false, false};
      }
      return EvalResult{false, EvaluatorUtils::compareVersions(valueAsString.value(), targetAsString.value(), op)};
    }
    else if (op == "any")
    {
      if (!valueAsString || !targetAsVector)
      {
        return EvalResult{false, false};
      }
      std::function<bool(std::string, std::string)> compareFn =
          [op](std::string a, std::string b) -> bool
      { return boost::iequals(a, b); };
      return EvalResult{false, EvaluatorUtils::vectorAny(valueAsString.value(), targetAsVector.value(), compareFn)};
    }
    else if (op == "none")
    {
      if (!valueAsString || !targetAsVector)
      {
        return EvalResult{false, false};
      }
      std::function<bool(std::string, std::string)> compareFn =
          [op](std::string a, std::string b) -> bool
      { return boost::iequals(a, b); };
      return EvalResult{false, !EvaluatorUtils::vectorAny(valueAsString.value(), targetAsVector.value(), compareFn)};
    }
    else if (op == "any_case_sensitive")
    {
      if (!valueAsString || !targetAsVector)
      {
        return EvalResult{false, false};
      }
      std::function<bool(std::string, std::string)> compareFn =
          [op](std::string a, std::string b) -> bool
      { return boost::equals(a, b); };
      return EvalResult{false, EvaluatorUtils::vectorAny(valueAsString.value(), targetAsVector.value(), compareFn)};
    }
    else if (op == "none_case_sensitive")
    {
      if (!valueAsString || !targetAsVector)
      {
        return EvalResult{false, false};
      }
      std::function<bool(std::string, std::string)> compareFn =
          [op](std::string a, std::string b) -> bool
      { return boost::equals(a, b); };
      return EvalResult{false, !EvaluatorUtils::vectorAny(valueAsString.value(), targetAsVector.value(), compareFn)};
    }
    else if (op == "str_starts_with_any")
    {
      if (!valueAsString || !targetAsVector)
      {
        return EvalResult{false, false};
      }
      std::function<bool(std::string, std::string)> compareFn =
          [op](std::string a, std::string b) -> bool
      { return boost::istarts_with(a, b); };
      return EvalResult{false, EvaluatorUtils::vectorAny(valueAsString.value(), targetAsVector.value(), compareFn)};
    }
    else if (op == "str_ends_with_any")
    {
      if (!valueAsString || !targetAsVector)
      {
        return EvalResult{false, false};
      }
      std::function<bool(std::string, std::string)> compareFn =
          [op](std::string a, std::string b) -> bool
      { return boost::iends_with(a, b); };
      return EvalResult{false, EvaluatorUtils::vectorAny(valueAsString.value(), targetAsVector.value(), compareFn)};
    }
    else if (op == "str_contains_any")
    {
      if (!valueAsString || !targetAsVector)
      {
        return EvalResult{false, false};
      }
      std::function<bool(std::string, std::string)> compareFn =
          [op](std::string a, std::string b) -> bool
      { return boost::icontains(a, b); };
      return EvalResult{false, EvaluatorUtils::vectorAny(valueAsString.value(), targetAsVector.value(), compareFn)};
    }
    else if (op == "str_contains_none")
    {
      if (!valueAsString || !targetAsVector)
      {
        return EvalResult{false, false};
      }
      std::function<bool(std::string, std::string)> compareFn =
          [op](std::string a, std::string b) -> bool
      { return boost::icontains(a, b); };
      return EvalResult{false, !EvaluatorUtils::vectorAny(valueAsString.value(), targetAsVector.value(), compareFn)};
    }
    else if (op == "str_matches")
    {
      if (!valueAsString || !targetAsString)
      {
        return EvalResult{false, false};
      }
      boost::regex targetAsRegExp(targetAsString.value());
      return EvalResult{false, boost::regex_search(valueAsString.value(), targetAsRegExp)};
    }
    else if (op == "eq")
    {
      return EvalResult{false, value == target};
    }
    else if (op == "neq")
    {
      return EvalResult{false, !(value == target)};
    }
    else if (op == "before")
    {
      return EvalResult{false, valueAsTime < targetAsTime};
    }
    else if (op == "after")
    {
      return EvalResult{false, valueAsTime > targetAsTime};
    }
    else if (op == "on")
    {
      return EvalResult{false, valueAsTime.date() == targetAsTime.date()};
    }
    // TODO: support id lists

    return EvalResult{true};
  }

  std::optional<EvalResult> Evaluator::evaluateDelegate(
      User user, ConfigRule rule,
      std::vector<std::unordered_map<std::string, std::string>> exposures)
  {
    return std::nullopt;
  }

  bool Evaluator::evalPassPercentage(User user, ConfigRule rule, ConfigSpec spec)
  {
    std::string hashString = spec.salt + "." + rule.salt + "." + getUnitID(user, rule.idType);
    std::string hash = Utils::sha256(hashString);
    auto hashEncoded = Utils::base64Encode(hash);
    return (hashEncoded % 10000) < rule.passPercentage * 100;
  }

  std::optional<JSON::any> Evaluator::getFromUser(User user, std::string field)
  {
    std::optional<JSON::any> value;
    std::string fieldLower = boost::algorithm::to_lower_copy(field);
    if (fieldLower == "userid" || fieldLower == "user_id")
    {
      value = user.userID;
    }
    else if (fieldLower == "email")
    {
      value = user.email;
    }
    else if (fieldLower == "ip" || fieldLower == "ipaddress" || fieldLower == "ip_address")
    {
      value = user.ipAddress;
    }
    else if (fieldLower == "useragent" || fieldLower == "user_agent")
    {
      value = user.userAgent;
    }
    else if (fieldLower == "country")
    {
      value = user.country;
    }
    else if (fieldLower == "locale")
    {
      value = user.locale;
    }
    else if (fieldLower == "appversion" || fieldLower == "app_version")
    {
      value = user.appVersion;
    }

    std::string valueAsString = Utils::typeSafeGet<std::string>(value).value_or("");
    if (valueAsString == "")
    {
      if (auto customValue = Utils::safeGetMap<std::string, JSON::any>(user.custom, field))
      {
        value = customValue;
      }
      else if (auto customValue =
                   Utils::safeGetMap<std::string, JSON::any>(user.custom, fieldLower))
      {
        value = customValue;
      }
      else if (auto privateValue =
                   Utils::safeGetMap<std::string, JSON::any>(user.privateAttribute, field))
      {
        value = privateValue;
      }
      else if (auto privateValue =
                   Utils::safeGetMap<std::string, JSON::any>(user.privateAttribute, fieldLower))
      {
        value = privateValue;
      }
    }
    return value;
  }

  std::optional<JSON::any> Evaluator::getFromUserAgent(User user, std::string field)
  {
    return std::nullopt;
  }
  std::optional<JSON::any> Evaluator::getFromEnvironment(User user, std::string field)
  {
    return std::nullopt;
  }

  std::string Evaluator::getUnitID(User user, std::string idType)
  {
    std::string idTypeLower = boost::algorithm::to_lower_copy(idType);
    if (idType != "" && idTypeLower != "userid")
    {
      if (auto customID = Utils::safeGetMap<std::string, JSON::any>(user.custom, idType); customID)
      {
        return Utils::typeSafeGet<std::string>(customID).value_or("");
      }
      if (auto customID = Utils::safeGetMap<std::string, JSON::any>(user.custom, idTypeLower);
          customID)
      {
        return Utils::typeSafeGet<std::string>(customID).value_or("");
      }
    }
    return user.userID;
  }

  void Evaluator::shutdown()
  {
    this->store->shutdown();
    this->store = NULL;
  }
}
