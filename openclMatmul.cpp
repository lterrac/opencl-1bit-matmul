#include <CL/opencl.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

#include "pack.hpp"
#include "print.hpp"
#include "matrixIO.hpp"
#include "check.hpp"

/**
 * Reads a file into a string.
 *
 * @param path path to the file.
 * @return String containing the full contents of the file.
 */
std::string readFromFile(const std::string &path)
{
  std::string source;
  // get size of file to know how much memory to allocate
  std::uintmax_t filesize = std::filesystem::file_size(path);
  source.resize(filesize);

  std::ifstream fin(path);
  fin.read(source.data(), filesize);

  check(!fin, "Error reading file could only read " + std::to_string(fin.gcount()) + " bytes");
  fin.close();
  return source;
}

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << std::string(argv[0]) << " <kernel_path> <test_directory>" << std::endl;
    std::cerr << "  This program runs OpenCL matrix multiplication and compares with ground truth." << std::endl;
    std::cerr << "  The test directory should contain a.bin, b.bin, and ground_truth.bin files." << std::endl;
    return -1;
  }

  std::string kernelPath = argv[1];
  std::string testDir    = argv[2];

  std::string matrixAFile     = testDir + "/a.bin";
  std::string matrixBFile     = testDir + "/b.bin";
  std::string groundTruthFile = testDir + "/ground_truth.bin";

  std::cout << "Loading matrices from files..." << std::endl;

  // Load matrices from files
  std::size_t M_A, K_A, K_B, N_B;
  auto        A                 = loadMatrix<uint8_t>(matrixAFile, M_A, K_A);
  auto        B                 = loadMatrix<uint8_t>(matrixBFile, K_B, N_B);
  auto        outputGroundTruth = loadMatrix<int>(groundTruthFile, M_A, N_B);

  std::cout << "Loaded matrices:" << std::endl;
  std::cout << "  Matrix A: " << M_A << "x" << K_A << std::endl;
  std::cout << "  Matrix B: " << K_B << "x" << N_B << std::endl;
  std::cout << "  Ground Truth: " << M_A << "x" << N_B << std::endl;

  // Verify dimensions are compatible
  check(K_A != K_B, "Error: Incompatible matrix dimensions. K_A=" + std::to_string(K_A) + ", K_B=" + std::to_string(K_B));

  const std::size_t M = M_A;
  const std::size_t N = N_B;
  const std::size_t K = K_A;

  // Number of 32-bit words needed to store K bits per row/column
  const std::size_t chunks = bitsToWords32(K);

  // Mask for the last word to zero out unused bits if K is not a multiple of 32
  const uint32_t lastMask = (K % 32) ? ((1u << (K % 32)) - 1u) : 0xFFFFFFFFu;

  // Print input matrices
#ifdef PRINT_MATRICES
  printMatrix1Bit(A, M, K, "A");
  printMatrix1Bit(B, K, N, "B");
#endif

  // Pack bits into 32-bit words for OpenCL
  auto packedA = packBits(A, M, K, bitLayout::RowMajor);
  auto packedB = packBits(B, N, K, bitLayout::ColMajor);

  // Setup OpenCL platform, device, context, and queue
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);
  check(platforms.empty(), "Error: No OpenCL platforms found.");

  auto                    platform = platforms[0];
  std::vector<cl::Device> devices;
  platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
  check(devices.empty(), "Error: No GPU devices found on platform.");

  cl::Device       device = devices[0];
  cl::Context      context(device);
  cl::CommandQueue queue(context, device);

  std::cout << "Using OpenCL device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;

  // Load and build OpenCL kernel
  auto                 source = readFromFile(kernelPath);
  cl::Program::Sources sources;
  sources.push_back({source.c_str(), source.size()});
  cl::Program program(context, sources);
  check(program.build({device}) != CL_SUCCESS, "Error: OpenCL program build failed.");

  // Create OpenCL buffers (host memory shared for A and B)
  cl::Buffer ADevice(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(uint32_t) * packedA.size(), packedA.data());
  cl::Buffer BDevice(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(uint32_t) * packedB.size(), packedB.data());
  cl::Buffer outputDevice(context, CL_MEM_WRITE_ONLY, sizeof(int) * M * N);

  // Set kernel arguments
  cl::Kernel kernel(program, "matmul");
  kernel.setArg(0, ADevice);
  kernel.setArg(1, BDevice);
  kernel.setArg(2, outputDevice);
  kernel.setArg(3, M);
  kernel.setArg(4, N);
  kernel.setArg(5, K);
  kernel.setArg(6, chunks);
  kernel.setArg(7, lastMask);

  std::cout << "Launching OpenCL kernel..." << std::endl;

  // Launch kernel. Let OpenCL pick the local group size
  auto err = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(M, N), cl::NullRange);
  check(err != CL_SUCCESS, "Error: Failed to enqueue kernel. CL error code: " + std::to_string(err));

  // Wait for kernel to finish
  err = queue.finish();
  check(err != CL_SUCCESS, "Error: Failed to finalize the queue. CL error code: " + std::to_string(err));

  // Read back GPU results
  std::vector<int> outputHost(M * N);
  err = queue.enqueueReadBuffer(outputDevice, CL_TRUE, 0, sizeof(int) * outputHost.size(), outputHost.data());
  check(err != CL_SUCCESS, "Error: Failed to read buffer. CL error code: " + std::to_string(err));

  std::cout << "Kernel execution completed successfully!" << std::endl;

  // Print CPU and GPU results
#ifdef PRINT_MATRICES
  printMatrix(outputGroundTruth, M, N, "output GroundTruth");
  printMatrix(outputHost, M, N, "output GPU");
#endif

  // Verify GPU result matches CPU reference
  check(!std::equal(outputGroundTruth.begin(), outputGroundTruth.end(), outputHost.begin()), "The output does not match with the ground truth!");

  std::cout << "Success!" << std::endl;
  return 0;
}
