#include "store.h"

namespace statsig
{
  void Store::initialize() { Store::fetchConfigSpecs(); }

  void Store::shutdown() {}

  std::optional<ConfigSpec> Store::getGate(std::string gateName)
  {
    return Utils::safeGetMap<std::string, ConfigSpec>(this->featureGates, gateName);
  }

  std::optional<ConfigSpec> Store::getConfig(std::string configName)
  {
    return Utils::safeGetMap<std::string, ConfigSpec>(this->dynamicConfigs, configName);
  }

  std::optional<ConfigSpec> Store::getLayer(std::string layerName)
  {
    return Utils::safeGetMap<std::string, ConfigSpec>(this->layerConfigs, layerName);
  }

  std::optional<std::string> Store::getExperimentLayer(std::string experimentName)
  {
    return Utils::safeGetMap<std::string, std::string>(this->experimentToLayer, experimentName);
  }

  void Store::fetchConfigSpecs()
  {
    std::multimap<std::string, JSON::any> body = {
        {"statsigMetadata", StatsigMetadata},
        {"sinceTime", 0},
    };
    auto res = this->network->postRequest("/v1/download_config_specs", body);
    if (res->status != 200)
    {
      return;
    }
    try
    {
      nlohmann::json specsJSON = nlohmann::json::parse(res->body);
      Store::processSpecsJSON(specsJSON);
    }
    catch (nlohmann::json::parse_error &e)
    {
      // TODO: Log SDK exception
      std::cout << e.what() << std::endl;
    }
  }

  void Store::processSpecsJSON(nlohmann::json specsJSON)
  {
    try
    {
      if (!specsJSON.at("has_updates"))
      {
        return;
      }

      json_safe_deserialize(specsJSON.at("feature_gates"), this->featureGates);
      json_safe_deserialize(specsJSON.at("dynamic_configs"), this->dynamicConfigs);
      json_safe_deserialize(specsJSON.at("layer_configs"), this->layerConfigs);
      std::unordered_map<std::string, std::vector<std::string>> layerToExperiments;
      json_safe_deserialize(specsJSON.at("layers"), layerToExperiments);
      this->experimentToLayer = reverseLayerToExperimentsMapping(layerToExperiments);
    }
    catch (...)
    {
      // TODO: Log SDK exception
    }
  }

  std::unordered_map<std::string, std::string> Store::reverseLayerToExperimentsMapping(
      std::unordered_map<std::string, std::vector<std::string>> &map)
  {
    std::unordered_map<std::string, std::string> experimentToLayer;
    for (auto &[layerName, experiments] : map)
    {
      for (auto experimentName : experiments) {
        experimentToLayer.insert({experimentName, layerName});
      }
    }
    return experimentToLayer;
  }
}