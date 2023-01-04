#pragma once

#include <chrono>
#include <httplib.h>

#include "statsig_metadata.h"
#include "types.h"
#include "utils.h"
#include "json.h"

namespace statsig
{
  class Network
  {
  public:
    Network(std::string sdkKey, Options options) : sdkKey(sdkKey), api(options.api){};
    httplib::Result postRequest(std::string endpoint, std::multimap<std::string, JSON::any> body);
    httplib::Result postRequest(std::string endpoint, std::string body);
    void shutdown();

  private:
    std::string sdkKey;
    std::string api;
  };
}