#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include "utils.h"
#include "boost/date_time/posix_time/posix_time.hpp"

namespace statsig
{
  class EvaluatorUtils
  {
  public:
    template <typename T>
    static bool compare(T a, T b, std::string op, std::function<int(T, T)> fn)
    {
      if (op == "gt")
      {
        return fn(a, b) > 0;
      }
      else if (op == "gte")
      {
        return fn(a, b) >= 0;
      }
      else if (op == "lt")
      {
        return fn(a, b) < 0;
      }
      else if (op == "lte")
      {
        return fn(a, b) <= 0;
      }
      else if (op == "eq")
      {
        return fn(a, b) == 0;
      }
      else if (op == "neq")
      {
        return fn(a, b) != 0;
      }
      return false;
    }

    static int compareVersionsHelper(std::string versionA, std::string versionB)
    {
      std::vector<std::string> partsA;
      std::vector<std::string> partsB;
      boost::split(partsA, versionA, boost::is_any_of("."));
      boost::split(partsB, versionB, boost::is_any_of("."));
      for (int i = 0; i < std::max(partsA.size(), partsB.size()); ++i)
      {
        int partA, partB = 0;
        if (i < partsA.size())
        {
          partA = stoi(partsA[i]);
        }
        if (i < partsB.size())
        {
          partB = stoi(partsB[i]);
        }
        if (partA < partB)
        {
          return -1;
        }
        if (partA > partB)
        {
          return 1;
        }
      }
      return 0;
    }

    static bool compareVersions(std::string a, std::string b, std::string op)
    {
      std::string versionA = a.substr(0, a.find("-"));
      std::string versionB = b.substr(0, b.find("-"));
      if (versionA.size() == 0 || versionB.size() == 0)
      {
        return false;
      }
      std::function<int(std::string, std::string)> compareFn = compareVersionsHelper;
      return compare(a, b, op.substr(op.find("version_") + 1), compareFn);
    }

    using Time = boost::posix_time::ptime;
    static Time getPosixTime(JSON::any v)
    {
      int time_maybe_ms;
      if (const int *maybeInt = std::get_if<int>(&v))
      {
        time_maybe_ms = *maybeInt;
      }
      else if (const long *maybeLong = std::get_if<long>(&v))
      {
        time_maybe_ms = *maybeLong;
      }
      else if (const long long *maybeLongLong = std::get_if<long long>(&v))
      {
        time_maybe_ms = *maybeLongLong;
      }
      else if (const float *maybeFloat = std::get_if<float>(&v))
      {
        time_maybe_ms = int(*maybeFloat);
      }
      else if (const double *maybeDouble = std::get_if<double>(&v))
      {
        time_maybe_ms = int(*maybeDouble);
      }
      else if (const std::string *maybeString = std::get_if<std::string>(&v))
      {
        try
        {
          time_maybe_ms = stoi(*maybeString);
        }
        catch (...)
        {
        }
      }

      boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
      if (std::to_string(time_maybe_ms).size() <= 11) // seconds
      {
        return epoch + boost::posix_time::milliseconds(time_maybe_ms * 1000);
      }
      else if (std::to_string(time_maybe_ms).size() <= 13) // miliseconds
      {
        return epoch + boost::posix_time::milliseconds(time_maybe_ms);
      }
      else // microseconds
      {
        return epoch + boost::posix_time::milliseconds(time_maybe_ms / 1000);
      }
    }

    template <typename T>
    static bool vectorAny(T value, std::vector<T> vector, std::function<bool(T, T)> fn)
    {
      for (auto v : vector)
      {
        if (fn(value, v))
        {
          return true;
        }
      }
      return false;
    }
  };
}