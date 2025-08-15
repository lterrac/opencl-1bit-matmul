#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <concepts>

#include "check.hpp"

/**
 * @brief Concept for numerical types that can be loaded from binary files.
 * 
 * This concept ensures that the template type T is a numerical type
 * that can be safely read from binary files using reinterpret_cast.
 */
template <typename T>
concept NumericalType = std::is_arithmetic_v<T>;

/**
 * @brief Loads a matrix from a binary file.
 *
 * @tparam T The numerical type of matrix elements (e.g., uint8_t, int)
 * @param filename The name of the file to load from
 * @param rows Reference to store the number of rows
 * @param cols Reference to store the number of columns
 * 
 * @return The loaded matrix data as a vector of type T
 */
template <NumericalType T>
inline std::vector<T> loadMatrix(const std::string &filename, std::size_t &rows, std::size_t &cols)
{
  std::ifstream file(filename, std::ios::binary);
  check(!file, "Error: Could not open file " + filename + " for reading");

  // Read dimensions
  file.read(reinterpret_cast<char *>(&rows), sizeof(rows));
  file.read(reinterpret_cast<char *>(&cols), sizeof(cols));

  // Calculate matrix size and read data
  std::size_t    matrixSize = rows * cols;
  std::vector<T> matrix(matrixSize);
  file.read(reinterpret_cast<char *>(matrix.data()), matrixSize * sizeof(T));
  file.close();

  return matrix;
}

/**
 * @brief Saves a matrix to a binary file.
 *
 * @tparam T The numerical type of matrix elements (e.g., uint8_t, int, float, double)
 * @param filename The name of the file to save to
 * @param matrix The matrix data to save
 * @param rows Number of rows in the matrix
 * @param cols Number of columns in the matrix
 */
template <NumericalType T>
void saveMatrix(const std::string &filename, const std::vector<T> &matrix, const std::size_t rows, const std::size_t cols)
{
  std::ofstream file(filename, std::ios::binary);
  check(!file, "Error: Could not open file " + filename + " for writing");

  // Write dimensions
  file.write(reinterpret_cast<const char *>(&rows), sizeof(rows));
  file.write(reinterpret_cast<const char *>(&cols), sizeof(cols));

  // Write matrix data
  file.write(reinterpret_cast<const char *>(matrix.data()), matrix.size() * sizeof(T));
  file.close();
}