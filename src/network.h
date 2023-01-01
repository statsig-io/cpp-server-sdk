#pragma once

#include <chrono>
#include <httplib.h>

#include "statsig_metadata.h"
#include "types.h"
#include "utils.h"

namespace statsig
{
  class Network
  {
  public:
    Network(std::string sdkKey, Options options) : api(options.api){};
    httplib::Result postRequest(std::string endpoint, std::multimap<std::string, std::any> body);
    httplib::Result postRequest(std::string endpoint, std::string body);

  private:
    std::string sdkKey;
    std::string api;
  };
}