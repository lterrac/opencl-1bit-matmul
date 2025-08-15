#pragma once

#include <cstdint>
#include <vector>

/**
 * @brief Specifies the memory layout of vector: RowMajor or ColMajor
 */
enum bitLayout
{
  RowMajor,
  ColMajor
};

/**
 * @brief Computes how many `uint32_t` words are needed to store `bits` bits,
 * rounding up to the nearest whole word.
 *
 * @param bits The number of bits to store.
 * @return The number of 32-bit words required.
 */
inline constexpr std::size_t bitsToWords32(std::size_t bits) noexcept
{
  // Add 31 to round up, then divide by 32 using right shift
  return (bits + 31u) >> 5u;
}

/**
 * @brief Packs a vector of bits into a vector of 32-bit words.
 *        Supports both row-major and column-major layouts.
 *
 * @param bits Input vector containing 0/1 bits.
 * @param dimension0 Number of rows (for row-major) or columns (for
 * column-major).
 * @param dimension1 Number of columns (for row-major) or rows (for
 * column-major)
 * @param layout Layout of the input bits: row-major or column-major.
 *
 * @return Packed 32-bit words containing the input bits.
 */
inline std::vector<uint32_t> packBits(const std::vector<uint8_t> &bits, int dimension0, int dimension1, bitLayout layout)
{
  // Output vector to store packed 32-bit words
  std::vector<uint32_t> out;

  // Compute how many 32-bit words (chunks) are needed to store the values in
  // dimension1
  int chunks = bitsToWords32(dimension1);

  // Allocate output vector and initialize all words to 0
  out.assign(dimension0 * chunks, 0u);

  for (int i = 0; i < dimension0; ++i)
  {
    for (int j = 0; j < dimension1; ++j)
    {
      // Pick the value based on the layout and convert it to 1-bit representation
      uint32_t bit;
      // Row-major: bit index = row * dimension1 + column
      if (layout == bitLayout::RowMajor) { bit = bits[i * dimension1 + j]; }
      // Column-major: bit index = column * dimension0 + row
      else { bit = bits[j * dimension0 + i]; }

      // Determine which 32-bit word this bit belongs to
      int chunk = j / 32;

      // Determine the bit position inside the 32-bit word
      int bitpos = j % 32;

      // Set the bit in the output vector using bitwise OR
      out[i * chunks + chunk] |= (bit << bitpos);
    }
  }

  // Return the packed vector of 32-bit words
  return out;
}