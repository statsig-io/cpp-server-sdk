#pragma once

#include <any>
#include <iostream>
#include <optional>
#include <maxminddb.h>

#ifdef PROJECT_ROOT_DIR
#define ROOT_DIR PROJECT_ROOT_DIR
#else
#define ROOT_DIR "/"
#endif

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
        std::cout << "Failed to open the MMDB file: " << filepath << std::endl;
      }
    };
    ~CountryLookup()
    {
      MMDB_close(&this->mmdb);
    }
    MMDB_lookup_result_s lookupRaw(const std::string ip)
    {
      int gai_error = 0; // get_address_info error
      int mmdb_error = 0;
      MMDB_lookup_result_s result = MMDB_lookup_string(&this->mmdb, ip.c_str(), &gai_error, &mmdb_error);
      if (gai_error)
      {
        std::cout << gai_strerror(gai_error) << std::endl;
      }
      if (mmdb_error)
      {
        std::cout << "Failed to lookup address: " << ip << std::endl;
      }
      if (!result.found_entry)
      {
      }
      return result;
    };
    /**
     * Look up which country the given IP address belongs to
     * @returns 2 letter country code (e.g. "US")
     */
    std::optional<std::string> lookupCountry(const std::string ip)
    {
      MMDB_lookup_result_s result = lookupRaw(ip);
      MMDB_entry_data_s entryData;
      const int status = MMDB_get_value(&result.entry, &entryData, "country", "iso_code");
      if (status != MMDB_SUCCESS)
      {
        std::cout << "Failed to get the country code: " << ip << std::endl;
        return std::nullopt;
      }
      return std::string(entryData.utf8_string);
    };

  private:
    MMDB_s mmdb;
  };
}