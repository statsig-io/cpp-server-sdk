#pragma once

#include <algorithm>
#include <any>
#include <map>
#include <optional>
#include <unordered_map>
#include <variant>

#include <boost/beast/core/detail/base64.hpp>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <openssl/sha.h>
#include <openssl/evp.h>

#include "json.h"

namespace statsig
{
  template <typename K, typename V>
  using Map = std::variant<std::map<K, V>, std::unordered_map<K, V>, std::multimap<K, V>>;

  class Utils
  {
  private:
    template <typename K, typename V>
    static std::optional<V> safeGetMapGeneric(Map<K, V> map, K key)
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
  public:
    // Individually defined to allow type inference
    template <typename K, typename V>
    static std::optional<V> safeGetMap(std::map<K, V> map, K key)
    {
      return Utils::safeGetMapGeneric<K, V>(map, key);
    };

    template <typename K, typename V>
    static std::optional<V> safeGetMap(std::unordered_map<K, V> map, K key)
    {
      return Utils::safeGetMapGeneric<K, V>(map, key);
    };

    template <typename K, typename V>
    static std::optional<V> safeGetMap(std::multimap<K, V> map, K key)
    {
      return Utils::safeGetMapGeneric<K, V>(map, key);
    };

    template <typename T, typename... Ts>
    static std::optional<T> typeSafeGet(std::optional<std::variant<Ts...>> value)
    {
      if (!value)
      {
        return std::nullopt;
      }
      return typeSafeGet<T>(value.value());
    }

    template <typename T, typename... Ts>
    static std::optional<T> typeSafeGet(std::variant<Ts...> value)
    {
      if (const T *maybeValue = std::get_if<T>(&value))
      {
        return *maybeValue;
      }
      return std::nullopt;
    }

    static std::string genUUIDString()
    {
      boost::uuids::uuid uuid = boost::uuids::random_generator()();
      return boost::uuids::to_string(uuid);
    };

    template <typename T>
    static std::string toJsonString(T object)
    {
      nlohmann::json j = object;
      return j.dump();
    }

    static long long getCurrentTimeMS()
    {
      return std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::system_clock::now().time_since_epoch())
          .count();
    }

    static std::string sha256(const std::string &unhashed)
    {
      std::string hashed;
      EVP_MD_CTX *context = EVP_MD_CTX_new();
      if (context != NULL)
      {
        if (EVP_DigestInit_ex(context, EVP_sha256(), NULL))
        {
          if (EVP_DigestUpdate(context, unhashed.c_str(), unhashed.length()))
          {
            unsigned char hash[EVP_MAX_MD_SIZE];
            unsigned int lengthOfHash = 0;

            if (EVP_DigestFinal_ex(context, hash, &lengthOfHash))
            {
              std::stringstream ss;
              for (unsigned int i = 0; i < lengthOfHash; ++i)
              {
                ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
              }

              hashed = ss.str();
            }
          }
        }

        EVP_MD_CTX_free(context);
      }
      return hashed;
    }

    static long long base64Encode(const std::string str)
    {
      size_t size = 16;
      void *encoded = malloc(boost::beast::detail::base64::encoded_size(size));
      boost::beast::detail::base64::encode(encoded, &str, size);
      return (long long)encoded;
    }

    static boost::thread spawnBackgroundThread(std::function<void()> fn)
    {
      boost::thread thread(fn);
      return thread;
    }
  };
}