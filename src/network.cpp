#include "network.h"

namespace statsig
{
  void Network::shutdown()
  {
  }

  httplib::Result Network::postRequest(std::string endpoint, std::multimap<std::string, JSON::any> body)
  {
    return Network::postRequest(endpoint, Utils::mapToJsonString<std::string, JSON::any>(body));
  }

  httplib::Result Network::postRequest(std::string endpoint, std::string body)
  {
    httplib::Client client(this->api);
    auto currentTime = Utils::getCurrentTimeMS();
    httplib::Headers headers = {
        {"STATSIG-API-KEY", this->sdkKey},
        {"STATSIG-CLIENT-TIME", std::to_string(currentTime)},
        {"STATSIG-SERVER-SESSION-ID", Utils::genUUIDString()},
        {"STATSIG-SDK-TYPE", StatsigMetadata.sdkType},
        {"STATSIG-SDK-VERSION", StatsigMetadata.sdkVersion},
    };
    return client.Post(endpoint, headers, body, "application/json");
  }
}