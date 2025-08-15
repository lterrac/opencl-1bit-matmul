#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

#include "groundTruth.hpp"
#include "matrixGenerator.hpp"
#include "matrixIO.hpp"
#include "pack.hpp"
#include "check.hpp"
#include <concepts>

int main(int argc, char **argv)
{
  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << " <M> <N> <K>" << std::endl;
    std::cerr << "  M: Number of rows in matrix A and output C" << std::endl;
    std::cerr << "  N: Number of columns in matrix B and output C" << std::endl;
    std::cerr << "  K: Shared dimension (columns of A, rows of B)" << std::endl;
    return -1;
  }

  const std::size_t M = std::stoul(argv[1]); // Number of rows in matrix A and output C
  const std::size_t N = std::stoul(argv[2]); // Number of columns in matrix B and output C
  const std::size_t K = std::stoul(argv[3]); // Shared dimension (columns of A, rows of B)

  // Hardcode the test directory to "tests"
  std::string baseOutputDir = "tests";

  // Create base output directory if it doesn't exist
  std::filesystem::create_directories(baseOutputDir);

  // Find the next available test ID
  int         testId = 0;
  std::string testDir;
  do {
    testId++;
    testDir = baseOutputDir + "/test_" + std::to_string(testId);
  }
  while (std::filesystem::exists(testDir));

  // Create the test directory
  std::filesystem::create_directories(testDir);

  std::cout << "Creating test directory: " << testDir << std::endl;

  std::cout << "Generating test matrices with dimensions:" << std::endl;
  std::cout << "  M (rows of A, rows of C): " << M << std::endl;
  std::cout << "  N (cols of B, cols of C): " << N << std::endl;
  std::cout << "  K (cols of A, rows of B): " << K << std::endl;

  // Instantiate random matrix generator (values 0 or 1)
  auto matrixGenerator = MatrixGenerator(0, 1);

  // Generate random M x K and K x N 1-bit matrices
  std::vector<uint8_t> A(M * K);
  matrixGenerator.populateMatrix(A, M, K);
  std::vector<uint8_t> B(K * N);
  matrixGenerator.populateMatrix(B, K, N);

  std::cout << "Generated matrices A (" << M << "x" << K << ") and B (" << K << "x" << N << ")" << std::endl;

  // Compute CPU reference (-1/+1) result
  auto outputGroundTruth = groundTruth(M, N, K, A, B);
  std::cout << "Computed ground truth result (" << M << "x" << N << ")" << std::endl;

  // Save matrices to files
  std::string matrixAFile     = testDir + "/a.bin";
  std::string matrixBFile     = testDir + "/b.bin";
  std::string groundTruthFile = testDir + "/ground_truth.bin";

  saveMatrix<uint8_t>(matrixAFile, A, M, K);
  saveMatrix<uint8_t>(matrixBFile, B, K, N);
  saveMatrix<int>(groundTruthFile, outputGroundTruth, M, N);

  std::cout << "Saved matrices to:" << std::endl;
  std::cout << "  Matrix A: " << matrixAFile << std::endl;
  std::cout << "  Matrix B: " << matrixBFile << std::endl;
  std::cout << "  Ground Truth: " << groundTruthFile << std::endl;
  std::cout << "Test ID: " << testId << std::endl;

  return 0;
}
