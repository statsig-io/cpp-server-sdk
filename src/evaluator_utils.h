#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

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

    static bool compareDates()
    {
      // TODO: compare dates
      return false;
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