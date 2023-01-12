#pragma once

#include "types.h"
#include "network.h"
#include "utils.h"

namespace statsig
{
  class EventLogger
  {
  public:
    EventLogger(Network *network, Options options)
    {
      this->network = network;
      this->options = options;
      this->queue = std::vector<Event>();
      this->flushBgThread = std::nullopt;
      if (!options.localMode)
      {
        this->flushBgThread = Utils::spawnBackgroundThread([this]()
                                                           { periodicFlush(); });
      }
    }
    void logEvent(Event event);
    void logGateExposure(User user, std::string gateName, EvalResult evaluation, bool isManualExposure);
    void logConfigExposure(User user, std::string configName, EvalResult evaluation, bool isManualExposure);
    void logLayerExposure(User user, std::string layerName, std::string parameterName, EvalResult evaluation, bool isManualExposure);
    void shutdown();

  private:
    Network *network;
    Options options;
    std::vector<Event> queue;
    std::optional<boost::thread> flushBgThread;
    void flush();
    void periodicFlush();
  };
}