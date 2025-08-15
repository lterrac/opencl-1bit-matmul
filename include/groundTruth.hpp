#pragma once

#include <cstdint>
#include <vector>

/**
 * @brief Computes the reference matrix multiplication for 1-bit matrices.
 *
 * This function performs a CPU-side "ground truth" computation for matrices represented as bits.
 * Each bit is interpreted as +1 if 1, -1 if 0, and the result is accumulated in `outputGroundTruth`.
 *
 * @param M Number of rows in matrix A and outputGroundTruth.
 * @param N Number of columns in matrix B and outputGroundTruth.
 * @param K Common dimension of matrices A and B.
 * @param A Input matrix A in 1-bit representation (row-major).
 * @param B Input matrix B in 1-bit representation (column-major).
 * 
 * @return outputGroundTruth Output reference matrix (int), size M x N.
 */
inline std::vector<int> groundTruth(const int M, const int N, const int K, std::vector<uint8_t> &A, std::vector<uint8_t> &B)
{
  std::vector<int> outputGroundTruth(M * N, 0);
  for (int r = 0; r < M; ++r)
  {
    for (int c = 0; c < N; ++c)
    {
      int sum = 0;
      for (int k = 0; k < K; ++k)
      {
        int a = A[r * K + k] ? +1 : -1;
        int b = B[k * N + c] ? +1 : -1;
        sum += a * b;
      }
      outputGroundTruth[r * N + c] = sum;
    }
  }
  return outputGroundTruth;
}