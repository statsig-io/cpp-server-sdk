// #include "utils.h"

// namespace statsig
// {
//   template <typename K, typename V>
//   std::optional<V> Utils::safeGetMap(Map<K, V> map, K key)
//   {
//     try
//     {
//       if (const auto maybeMap = std::get_if<std::map<K, V>>(&map))
//       {
//         return maybeMap.at(key);
//       }
//       else if (const auto maybeUnorderedMap = std::get<std::unordered_map<K, V>>(map))
//       {
//         return maybeUnorderedMap.at(key);
//       }
//       else if (const auto maybeMultimap = std::get<std::multimap<K, V>>(map))
//       {
//         for (auto it = maybeMultimap.find(key); it != maybeMultimap.end(); it++)
//         {
//           return it->second;
//         }
//         return std::nullopt;
//       }
//     }
//     catch (std::out_of_range)
//     {
//       return std::nullopt;
//     }
//   }

//   std::string Utils::genUUIDString()
//   {
//     boost::uuids::uuid uuid = boost::uuids::random_generator()();
//     return boost::uuids::to_string(uuid);
//   }

//   template <typename K, typename V>
//   std::string Utils::mapToJsonString(Map<K, V> map)
//   {
//     json json(map);
//     return json.dump();
//   }
// }