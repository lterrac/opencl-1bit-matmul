#pragma once

#include <random>
#include <vector>

/**
 * Utility class to generate random integer matrices.
 */
class MatrixGenerator
{
  public:

  /**
     * @brief Constructs a random matrix generator with given bounds.
     *
     * @param lowerBound The minimum value (inclusive) for generated integers.
     * @param upperBound The maximum value (inclusive) for generated integers.
     */
  MatrixGenerator(int lowerBound, int upperBound)
    : _generator(std::random_device().operator()()),
      _distribution(std::uniform_int_distribution<int>(lowerBound, upperBound))
  {}

  /**
     * @brief Populates a matrix with random integers in the specified range.
     *
     * @param matrix Reference to a 1D vector representing the matrix.
     * @param dimension0 First dimension.
     * @param dimension1 Second dimension.
     */
  inline void populateMatrix(std::vector<uint8_t> &matrix, std::size_t dimension0, std::size_t dimension1)
  {
    for (std::size_t i = 0; i < dimension0 * dimension1; ++i) matrix[i] = _distribution(_generator); // Fill each element with a random value
  }

  private:

  std::mt19937                       _generator;    ///< Mersenne Twister RNG engine
  std::uniform_int_distribution<int> _distribution; ///< Distribution for generating integers in [lowerBound, upperBound]
};