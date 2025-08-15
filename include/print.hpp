#pragma once

#include <iostream>
#include <vector>

inline void printMatrix1Bit(const std::vector<uint8_t> &bits, int rows, int cols, const std::string &name)
{
  std::cout << name << " (" << rows << "x" << cols << ") bits:\n";
  for (int r = 0; r < rows; ++r)
  {
    for (int c = 0; c < cols; ++c) { std::cout << int(bits[r * cols + c]) << " "; }
    std::cout << "\n";
  }
}

inline void printMatrix(const std::vector<int> &mat, int rows, int cols, const std::string &name)
{
  std::cout << name << " (" << rows << "x" << cols << "):\n";
  for (int r = 0; r < rows; ++r)
  {
    for (int c = 0; c < cols; ++c) { std::cout << std::setw(4) << mat[r * cols + c]; }
    std::cout << "\n";
  }

  // 1-bit representation (threshold at 0)
  std::cout << name << " (" << rows << "x" << cols << ") bits:\n";
  for (int r = 0; r < rows; ++r)
  {
    for (int c = 0; c < cols; ++c)
    {
      int bit = (mat[r * cols + c] >= 0) ? 1 : 0;
      std::cout << bit << " ";
    }
    std::cout << "\n";
  }
}