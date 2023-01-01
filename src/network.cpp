#include "network.h"

namespace statsig
{
  httplib::Result Network::postRequest(std::string endpoint, std::multimap<std::string, std::any> body)
  {
    return Network::postRequest(endpoint, Utils::mapToJsonString<std::string, std::any>(body));
  }

  httplib::Result Network::postRequest(std::string endpoint, std::string body)
  {
    httplib::Client client(api);
    auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::system_clock::now().time_since_epoch())
                           .count();
    httplib::Headers headers = {
        {"STATSIG-API-KEY", sdkKey},
        {"STATSIG-CLIENT-TIME", std::to_string(currentTime)},
        {"STATSIG-SERVER-SESSION-ID", Utils::genUUIDString()},
        {"STATSIG-SDK-TYPE", StatsigMetadata.sdkType},
        {"STATSIG-SDK-VERSION", StatsigMetadata.sdkVersion},
    };
    return client.Post(endpoint, headers, body, "application/json");
  }
}