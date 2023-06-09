#include "event_logger.h"

namespace statsig
{
  void EventLogger::logEvent(Event event)
  {
    if (this->options.localMode)
    {
      return;
    }
    event.time = Utils::getCurrentTimeMS();
    enqueue(event);
    RLock rLock(this->mu);
    if (this->queue.size() >= this->options.loggingMaxBufferSize)
    {
      rLock.unlock();
      flush();
    }
  }

  void EventLogger::logGateExposure(User user, std::string gateName, EvalResult evaluation, bool isManualExposure)
  {
    Event event{"statsig::gate_exposure", user};
    event.metadata = {
        {"gate", gateName},
        {"gateValue", evaluation.booleanValue ? "true" : "false"},
        {"ruleID", evaluation.ruleID},
    };
    if (isManualExposure)
    {
      event.metadata.insert({"isManualExposure", "true"});
    }
    event.secondaryExposures = evaluation.secondaryExposures;

    this->logEvent(event);
  }

  void EventLogger::logConfigExposure(User user, std::string configName, EvalResult evaluation, bool isManualExposure)
  {
    Event event{"statsig::config_exposure", user};
    event.metadata = {
        {"config", configName},
        {"ruleID", evaluation.ruleID},
    };
    if (isManualExposure)
    {
      event.metadata.insert({"isManualExposure", "true"});
    }
    event.secondaryExposures = evaluation.secondaryExposures;

    this->logEvent(event);
  }

  void EventLogger::logLayerExposure(User user, Layer layer, std::string parameterName, EvalResult evaluation, bool isManualExposure)
  {
    std::string allocatedExperiment = "";
    auto exposures = evaluation.undelegatedSecondaryExposures;
    auto explicitParameters = evaluation.explicitParameters.value_or(std::vector<std::string>());
    bool isExplicit = std::find(explicitParameters.begin(), explicitParameters.end(), parameterName) != explicitParameters.end();
    if (isExplicit)
    {
      exposures = evaluation.secondaryExposures;
      allocatedExperiment = evaluation.configDelegate;
    }
    Event event{"statsig::layer_exposure", user};
    event.metadata = {
        {"config", layer.name},
        {"ruleID", layer.ruleID},
        {"allocatedExperiment", allocatedExperiment},
        {"parameterName", parameterName},
        {"isExplicitParameter", isExplicit ? "true" : "false"},
    };
    if (isManualExposure)
    {
      event.metadata.insert({"isManualExposure", "true"});
    }
    event.secondaryExposures = exposures;

    this->logEvent(event);
  }

  void EventLogger::enqueue(Event event) {
    WLock lock(this->mu);
    this->queue.push_back(event);
  }

  void EventLogger::flush()
  {
    RLock rLock(this->mu);
    if (this->queue.empty())
    {
      return;
    }
    rLock.unlock();
    WLock wLock(this->mu);
    std::multimap<std::string, JSON::serializable> body = {
        {"events", this->queue},
        {"statsigMetadata", StatsigMetadata},
    };
    this->queue.clear();
    auto res = this->network->postRequest("/v1/log_event", body);
  }

  void EventLogger::periodicFlush()
  {
    while (true)
    {
      boost::this_thread::sleep_for(boost::chrono::milliseconds(this->options.loggingIntervalMs));
      flush();
    }
  }

  void EventLogger::shutdown()
  {
    flush();
    if (this->flushBgThread)
    {
      this->flushBgThread->interrupt();
      this->flushBgThread->join();
    }
  }
}