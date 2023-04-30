#pragma once

#include "types.h"
#include "network.h"
#include "utils.h"
#include "layer.h"

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
    ~EventLogger() { shutdown(); };
    void logEvent(Event event);
    void logGateExposure(User user, std::string gateName, EvalResult evaluation, bool isManualExposure);
    void logConfigExposure(User user, std::string configName, EvalResult evaluation, bool isManualExposure);
    void logLayerExposure(User user, Layer layer, std::string parameterName, EvalResult evaluation, bool isManualExposure);
    void shutdown();

  private:
    Network *network;
    Options options;
    std::shared_mutex mu;
    std::vector<Event> queue;
    std::optional<boost::thread> flushBgThread;
    void enqueue(Event event);
    void flush();
    void periodicFlush();
  };
}