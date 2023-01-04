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
    static std::optional<V> safeGetMap(Map<K, V> map, K key)
    {
      try
      {
        if (const std::map<K, V> *maybeMap = std::get_if<std::map<K, V>>(&map))
        {
          return maybeMap->at(key);
        }
        else if (const std::unordered_map<K, V> *maybeUnorderedMap = std::get_if<std::unordered_map<K, V>>(&map))
        {
          return maybeUnorderedMap->at(key);
        }
        else if (const std::multimap<K, V> *maybeMultimap = std::get_if<std::multimap<K, V>>(&map))
        {
          for (auto it = maybeMultimap->find(key); it != maybeMultimap->end(); it++)
          {
            return it->second;
          }
          return std::nullopt;
        }
        return std::nullopt;
      }
      catch (std::out_of_range)
      {
        return std::nullopt;
      }
    };

    static std::string genUUIDString()
    {
      boost::uuids::uuid uuid = boost::uuids::random_generator()();
      return boost::uuids::to_string(uuid);
    };

    template <typename K, typename V>
    static std::string mapToJsonString(Map<K, V> map)
    {
      nlohmann::json jsonObj = map;
      return jsonObj.dump();
    };
  };
}