#pragma once

#include <iostream>
#include <string>

/**
 * @brief Checks a condition and prints an error message if the condition is true, then exits.
 *
 * @param condition The boolean condition to check.
 * @param message The error message to display if the condition is true.
 */
inline void check(bool condition, const std::string &message) noexcept
{
  if (condition == false) { return; }
  std::cerr << message << std::endl;
  exit(-1);
}
