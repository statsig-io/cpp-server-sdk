#pragma once

#ifdef PROJECT_ROOT_DIR
#define ROOT_DIR PROJECT_ROOT_DIR
#else
#define ROOT_DIR "/"
#endif

#ifdef MAXMINDDB_DEFINED

#include <any>
#include <iostream>
#include <optional>
#include <maxminddb.h>

#include "output_logger.h"

namespace statsig
{
  class CountryLookup
  {
  public:
    CountryLookup(std::string filepath = std::string(ROOT_DIR) + "/src/data/GeoLite2-Country.mmdb")
    {
      const int status = MMDB_open(filepath.c_str(), MMDB_MODE_MMAP, &this->mmdb);
      if (status != MMDB_SUCCESS)
      {
        STATSIG_LOG("Failed to open the MMDB file:", filepath);
      }
    };
    ~CountryLookup()
    {
      MMDB_close(&this->mmdb);
    };
    /**
     * Look up which country the given IP address belongs to
     * @returns 2 letter country code (e.g. "US")
     */
    std::optional<std::string> lookupCountry(const std::string ip)
    {
      MMDB_lookup_result_s result = lookupRaw(ip);
      MMDB_entry_data_s entryData;
      const int status = MMDB_get_value(&result.entry, &entryData, "country", "iso_code", NULL);
      if (status != MMDB_SUCCESS || !entryData.has_data)
      {
        STATSIG_LOG("Failed to get the country code of:", ip, "Status:", status);
        return std::nullopt;
      }
      if (entryData.type != MMDB_DATA_TYPE_UTF8_STRING)
      {
        STATSIG_LOG("Country code is not a valid string. Type is", entryData.type);
        return std::nullopt;
      }
      std::string isoCode;
      isoCode.assign(entryData.utf8_string, entryData.utf8_string + entryData.data_size);
      return isoCode;
    };

  private:
    MMDB_s mmdb;
    MMDB_lookup_result_s lookupRaw(const std::string ip)
    {
      int gai_error = 0; // get_address_info error
      int mmdb_error = 0;
      MMDB_lookup_result_s result = MMDB_lookup_string(&this->mmdb, ip.c_str(), &gai_error, &mmdb_error);
      if (gai_error)
      {
        STATSIG_LOG(gai_strerror(gai_error));
      }
      if (mmdb_error)
      {
        STATSIG_LOG("Failed to lookup address:", ip);
      }
      if (!result.found_entry)
      {
        STATSIG_LOG("No entry found for address:", ip);
      }
      return result;
    };
  };
}
#else

#include <string>
#include <optional>

namespace statsig
{
  class CountryLookup
  {
  public:
    CountryLookup(){};
    ~CountryLookup(){};
    std::optional<std::string> lookupCountry(const std::string ip)
    {
      return std::nullopt;
    };
  };
}
#endif