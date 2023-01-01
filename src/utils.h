#pragma once

#include <any>
#include <map>
#include <optional>
#include <unordered_map>
#include <variant>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "json.h"

namespace statsig
{
  template <typename K, typename V>
  using Map = std::variant<std::map<K, V>, std::unordered_map<K, V>, std::multimap<K, V>>;

  class Utils
  {
  public:
    template <typename K, typename V>
    static std::optional<V> safeGetMap(Map<K, V> map, K key);
    static std::string genUUIDString();
    template <typename K, typename V>
    static std::string mapToJsonString(Map<K, V> map);
  };
}