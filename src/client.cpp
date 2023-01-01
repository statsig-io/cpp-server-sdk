#include "client.h"

namespace statsig
{
  bool Client::checkGate(User user, std::string gate)
  {
    // const res = evaluator->checkGate();
    // if (res.fetchFromServer)
    return false;
  }

  DynamicConfig Client::getConfig(User user, std::string config)
  {
    // return client->getConfig(user, config);
    return DynamicConfig();
  }

  void Client::shutdown()
  {
  }
}