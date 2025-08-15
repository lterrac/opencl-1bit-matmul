# OpenCL 1-bit Matrix Multiplication

GPU-accelerated 1-bit matrix multiplication using OpenCL with automated test generation.

The executables are 2:

1) **Test generator**: randomly generates the inputs and the expected output
2) **OpenCL Matmul**: loads the desired inputs, runs the computation using OpenCL, and compares it with the expected result

## Requirements

- Linux with OpenCL 3.0 support
- GPU with OpenCL drivers
- C++20 compiler (GCC 11+)
- Meson build system

## Build

```bash
meson setup build
meson compile -C build
```

## Usage

### Generate Tests
```bash
# Generate 5 tests with dimensions M=1024, N=1024, K=512
# The matrices have dimensions (M, K) and (K, N)
./generate_tests.sh 5 1024 1024 512

# Generate with custom dimensions
./generate_tests.sh 10 2048 1024 768
```

### Run Tests
```bash
# Simply execute all the tests cases
./run_tests.sh
```

As an alternative, it is possible to run the same tests within meson itself:

```bash 
# Reconfigure meson so it notices the new tests
meson setup --reconfigure build

# Run all tests
meson test -C build
```

Instead to manually run a single test:
```bash
# Run specific test
./build/openclMatmul ./kernels/matmul.cl tests/test_1
```

## Algorithm

1-bit matrices where each element is 0 or 1. Bits are interpreted as +1 (1) or -1 (0) for multiplication. Results are integer dot products. Implemented using:

```
Given A and B (1-bit vectors) of length K:

1) C = XNOR(A,B) -> equal to '==' operator
2) P = popcount(C) -> count the number of '1' bits
3) bipolar mapping: 2 * P - K -> final result
```

## Troubleshooting

To check the used input and outputs enable the corresponding flag when configuring meson:
```bash
# Enable debug output
meson setup build -DprintMatrices=true
```
