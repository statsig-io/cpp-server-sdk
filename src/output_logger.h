#pragma once

#include <iostream>

namespace statsig
{
  template <typename... Args>
  static void STATSIG_LOG(Args &&...args)
  {
    std::cout << "[Statsig]";
    ((std::cout << " " << std::forward<Args>(args)), ...);
    std::cout << std::endl;
  }
}