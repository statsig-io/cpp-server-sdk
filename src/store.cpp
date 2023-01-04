#include "store.h"

namespace statsig
{
  void Store::initialize()
  {
    Store::fetchConfigSpecs();
  }

  void Store::shutdown()
  {
  }

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
        // {"statsigMetadata", nlohmann::json::parse(StatsigMetadata)},
        {"statsigMetadata", StatsigMetadata},
        {"sinceTime", 0},
    };
    auto res = this->network->postRequest("/download_config_specs", body);
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

      // auto featureGatesListJson = specsJSON.at("feature_gates").get_to(this->featureGates);
      // auto featureGatesListJson = specsJSON.at("feature_gates").get<std::vector<ConfigSpec>>();
      // auto gatesList = featureGatesListJson.get<std::vector<ConfigSpec>>();
      // auto dynamicConfigsListJson = specsJSON.at("dynamic_configs");
      // auto layerConfigsListJson = specsJSON.at("layer_configs");
      // auto experimentToLayerJson = specsJSON.at("layers");

      this->featureGates = specsJSON.at("feature_gates").get_to(this->featureGates);
      this->dynamicConfigs = specsJSON.at("dynamic_configs").get_to(this->dynamicConfigs);
      this->layerConfigs = specsJSON.at("layer_configs").get_to(this->layerConfigs);
      this->experimentToLayer = specsJSON.at("layers").get_to(this->experimentToLayer); // TODO: reverse mapping
      std::cout << specsJSON << std::endl;
    }
    catch (...)
    {
      // TODO: Log SDK exception
    }
  }
}