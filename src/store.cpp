#include "store.h"

namespace statsig
{
  void Store::initialize() { Store::fetchConfigSpecs(); }

  void Store::shutdown() {
    if (this->syncBgThread)
    {
      this->syncBgThread->interrupt();
      this->syncBgThread->join();
    }
  }

  std::optional<ConfigSpec> Store::getGate(std::string gateName)
  {
    return Utils::safeGetMap(this->featureGates, gateName);
  }

  std::optional<ConfigSpec> Store::getConfig(std::string configName)
  {
    return Utils::safeGetMap(this->dynamicConfigs, configName);
  }

  std::optional<ConfigSpec> Store::getLayer(std::string layerName)
  {
    return Utils::safeGetMap(this->layerConfigs, layerName);
  }

  std::optional<std::string> Store::getExperimentLayer(std::string experimentName)
  {
    return Utils::safeGetMap(this->experimentToLayer, experimentName);
  }

  void Store::fetchConfigSpecs()
  {
    std::multimap<std::string, JSON::serializable> body = {
        {"statsigMetadata", StatsigMetadata},
        {"sinceTime", this->lastConfigSyncTime},
    };
    auto res = this->network->postRequest("/v1/download_config_specs", body);
    if (!res || res->status != 200)
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
      std::cout << "Error parsing download_config_specs body: " << e.what() << std::endl;
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
      auto newFeatureGates = std::unordered_map<std::string, ConfigSpec>();
      auto newDynamicConfigs = std::unordered_map<std::string, ConfigSpec>();
      auto newLayerConfigs = std::unordered_map<std::string, ConfigSpec>();
      json_safe_deserialize(specsJSON.at("feature_gates"), newFeatureGates);
      json_safe_deserialize(specsJSON.at("dynamic_configs"), newDynamicConfigs);
      json_safe_deserialize(specsJSON.at("layer_configs"), newLayerConfigs);
      this->featureGates = newFeatureGates;
      this->dynamicConfigs = newDynamicConfigs;
      this->layerConfigs = newLayerConfigs;
      std::unordered_map<std::string, std::vector<std::string>> layerToExperiments;
      json_safe_deserialize(specsJSON.at("layers"), layerToExperiments);
      this->experimentToLayer = reverseLayerToExperimentsMapping(layerToExperiments);
      json_safe_deserialize(specsJSON.at("time"), this->lastConfigSyncTime);
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
      for (auto experimentName : experiments)
      {
        experimentToLayer.insert({experimentName, layerName});
      }
    }
    return experimentToLayer;
  }

  void Store::sync()
  {
    while (true)
    {
      boost::this_thread::sleep_for(boost::chrono::milliseconds(this->options.rulesetsSyncIntervalMs));
      fetchConfigSpecs();
    }
  }
}